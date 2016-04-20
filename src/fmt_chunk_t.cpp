//
// Created by joelm on 2016-04-19.
//

#include <cstring>
#include <endian.h>
#include <malloc.h>
#include "fmt_chunk_t.h"

fmt_chunk_t::fmt_chunk_t(const char *data) {
    // Chunk ID
    memcpy(chunk_ID, data, 4);
    chunk_ID[4] = '\0';
    if (strcmp(chunk_ID, "fmt ") != 0) {
        throw "fmt chunk chunk ID is incorrect (should be \"fmt \")";
    }

    // Chunk Data Size
    memcpy(&chunk_data_size, data + 4, 4);
    chunk_data_size = le32toh(chunk_data_size);
    if (chunk_data_size != 0x10) {
        throw "fmt chunk data size is incorrect";
    }

    // Compression Code
    memcpy(&compression_code, data + 8, 2);
    compression_code = le16toh(compression_code);
    if (compression_code != 0x01) {
        throw "fmt chunk compression code is incorrect";
    }

    // Channel Numbers
    memcpy(&channel_numbers, data + 10, 2);
    channel_numbers = le16toh(channel_numbers);
    if (channel_numbers > 0x02) {
        throw "fmt chunk channel numbers is incorrect";
    }

    // Sample Rate
    memcpy(&sample_rate, data + 12, 4);
    sample_rate = le32toh(sample_rate);
    if (sample_rate == 0) {
        throw "fmt chunk sample rate cannot be 0";
    }

    // Bytes Per Second
    memcpy(&bytes_per_second, data + 16, 4);
    bytes_per_second = le32toh(bytes_per_second);
    if (bytes_per_second == 0) {
        throw "fmt chunk bytes per second cannot be 0";
    }

    // Bytes Per Sample
    memcpy(&bytes_per_sample, data + 20, 2);
    bytes_per_sample = le16toh(bytes_per_sample);
    if (bytes_per_sample == 0) {
        throw "fmt chunk bytes per sample cannot be 0";
    }

    // Bits Per Sample
    memcpy(&bits_per_sample, data + 22, 2);
    bits_per_sample = le16toh(bits_per_sample);
    if (bits_per_sample != 8 * bytes_per_sample / channel_numbers) {
        throw "fmt chunk bits per sample does not match the bytes per sample and channel count";
    }
}

// Don't forget to call free!
char *fmt_chunk_t::file_data() {
    char* out = (char*) malloc(SIZE);
    unsigned int temp32;
    unsigned short temp16;

    // Chunk ID
    memcpy(out, chunk_ID, 4);

    // Chunk Data Size
    temp32 = htole32(chunk_data_size);
    memcpy(out + 4, &temp32, 4);

    // Compression code
    temp16 = htole16(compression_code);
    memcpy(out + 8, &temp16, 2);

    // Channel Numbers
    temp16 = htole16(channel_numbers);
    memcpy(out + 10, &temp16, 2);

    // Sample rate
    temp32 = htole32(sample_rate);
    memcpy(out + 12, &temp32, 4);

    // Bytes Per Second
    temp32 = htole32(bytes_per_second);
    memcpy(out + 16, &temp32, 4);

    // Bytes Per Sample
    temp16 = htole16(bytes_per_sample);
    memcpy(out + 20, &temp16, 2);

    // Bits Per Sample
    temp16 = htole16(bits_per_sample);
    memcpy(out + 22, &temp16, 2);

    return out;
}