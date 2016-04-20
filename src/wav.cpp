//
// Created by joelm on 2016-04-19.
//

#include <cstring>
#include <fstream>
#include <iostream>
#include "wav.h"

wav::wav(std::ifstream& input) {
    char RIFF_type_chunk[riff_type_chunk_t::SIZE];
    char fmt_chunk[fmt_chunk_t::SIZE];
    char* data_chunk;
    unsigned int data_chunk_size;
    unsigned int cursor = 0;

    input.read(RIFF_type_chunk, riff_type_chunk_t::SIZE);
    cursor += input.gcount();
    if (input.gcount() != riff_type_chunk_t::SIZE) {
        throw "Problem reading the RIFF Type Chunk";
    }

    input.read(fmt_chunk, fmt_chunk_t::SIZE);
    cursor += input.gcount();
    if (input.gcount() != fmt_chunk_t::SIZE) {
        throw "Problem reading the fmt Chunk";
    }
    riff_type_ptr = new riff_type_chunk_t(RIFF_type_chunk);
    fmt_chunk_ptr = new fmt_chunk_t(fmt_chunk);

    do {
        data_chunk = (char *) malloc(8); // Enough for the headers
        input.read(data_chunk, 8);
        cursor += input.gcount();
        if (input.gcount() != 8) {
            throw "Error reading data chunk header";
        }
        data_chunk_size = data_chunk_t::read_chunk_size(data_chunk);
        data_chunk = (char*) realloc(data_chunk, data_chunk_size);
        input.read(data_chunk + 8, data_chunk_size - 8);
        cursor += input.gcount();
        if (input.gcount() != data_chunk_size - 8) {
            throw "Error reading data chunk";
        }
        data_chunks.push_back(new data_chunk_t(data_chunk));
    } while (cursor < riff_type_ptr->file_size);
}

wav::~wav() {
    delete riff_type_ptr;
    delete fmt_chunk_ptr;
}

long wav::sample(unsigned int index) {
    switch (fmt_chunk_ptr->bits_per_sample / 8) {
        case 1:
            return data_chunks[0]->data.d8[index];
        case 2:
            return data_chunks[0]->data.d16[index];
        case 4:
            return data_chunks[0]->data.d32[index];
        case 8:
            return data_chunks[0]->data.d64[index];
        default:
            throw "Unknown bytes per sample";
    }
}

void wav::set_sample(unsigned int index, long value) {
    value = value < max_amplitude() ? value : max_amplitude();
    switch (fmt_chunk_ptr->bits_per_sample / 8) {
        case 1:
            data_chunks[0]->data.d8[index] = (signed char) value;
            break;
        case 2:
            data_chunks[0]->data.d16[index] = (short) value;
            break;
        case 4:
            data_chunks[0]->data.d32[index] = (int) value;
            break;
        case 8:
            data_chunks[0]->data.d64[index] = value;
            break;
        default:
            throw "Unknown bytes per sample";
    }
}

long wav::max_amplitude() {
    return 1 << (fmt_chunk_ptr->bits_per_sample - 1);
}

long wav::find_max_sample() {
    unsigned int sample_c = sample_count();
    long max_sample = 0;
    long s;
    for (unsigned int i = 0; i < sample_c; i++) {
        s = sample(i);
        s = s < 0 ? -s : s;
        if (s > max_sample) {
            max_sample = s;
        }
    }
    return max_sample;
}

unsigned int wav::sample_count() {
    return data_chunks[0]->data_size() / (fmt_chunk_ptr->bits_per_sample / 8);
}

void wav::save(const char *file_name) {
    char* buffer;
    std::ofstream out(file_name, std::ios::out);

    // RIFF Type Chunk
    buffer = riff_type_ptr->file_data(data_chunks[0]->chunk_data_size);
    out.write(buffer, riff_type_chunk_t::SIZE);
    free(buffer);
    if (out.bad()) {
        out.close();
        throw "Error writing the RIFF header";
    }

    // fmt chunk
    buffer = fmt_chunk_ptr->file_data();
    out.write(buffer, fmt_chunk_t::SIZE);
    free(buffer);
    if (out.bad()) {
        out.close();
        throw "Error writing the fmt chunk";
    }

    // Data Chunk
    buffer = data_chunks[0]->file_data();
    out.write(buffer, data_chunks[0]->chunk_data_size);
    free(buffer);
    if (out.bad()) {
        out.close();
        throw "Error writing the data";
    }

    out.close();
}

void wav::amplify(float factor) {
    long new_val;
    for (unsigned int i = 0; i < sample_count(); i++) {
        new_val = (long) (sample(i) * factor);
        new_val = new_val < max_amplitude() ? new_val : max_amplitude();
        set_sample(i, new_val);
    }
}

void wav::reverse() {
    long temp;
    unsigned int far_end;
    for (unsigned int i = 0; i < sample_count() / 2; i++) {
        far_end = sample_count() - i;
        temp = sample(i);
        set_sample(i, sample(far_end));
        set_sample(far_end, temp);
    }
}

void wav::echo(float weight, unsigned int delay_samples) {
    long buffer[delay_samples];
    unsigned int buffer_index;
    long temp;
    expand(delay_samples);
    for (buffer_index = 0; buffer_index < delay_samples; buffer_index++) {
        buffer[buffer_index] = sample(buffer_index);
    }

    for (unsigned int i = buffer_index; i < sample_count(); i++) {
        temp = sample(i);
        temp += buffer[buffer_index] * weight;
        set_sample(i, temp);
        buffer[buffer_index] = temp;
        buffer_index = (buffer_index + 1) % delay_samples;
    }
}

void wav::expand(int samples) {
    data_chunks[0]->expand(samples * fmt_chunk_ptr->bits_per_sample / 8);
}

void wav::echo_seconds(float weight, float delay) {
    echo(weight, seconds_to_samples(delay));
}

unsigned int wav::seconds_to_samples(float seconds) {
    return (unsigned int) (seconds * fmt_chunk_ptr->sample_rate * fmt_chunk_ptr->bits_per_sample / 8);
}

void wav::trim_start(float seconds) {
    unsigned int samples = seconds_to_samples(seconds);
    if (samples > sample_count()) {
        throw "You are trying to trim more than the file's duration.";
    }
    for (unsigned int i = samples; i < sample_count(); i++) {
        set_sample(i - samples, sample(i));
    }
    expand(-samples);
}

void wav::trim_end(float seconds) {
    unsigned int samples = seconds_to_samples(seconds);
    if (samples > sample_count()) {
        throw "You are trying to trim more than the file's duration.";
    }
    expand(-samples);
}