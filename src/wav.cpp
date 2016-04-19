//
// Created by joelm on 2016-04-19.
//

#include <cstring>
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
        std::cout << "Chunk size: " << data_chunk_size << std::endl;
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