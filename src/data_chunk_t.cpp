//
// Created by joelm on 2016-04-19.
//

#include <cstring>
#include <endian.h>
#include <malloc.h>
#include "data_chunk_t.h"

unsigned int data_chunk_t::read_chunk_size(char *data) {
    unsigned int size;
    memcpy(&size, data + 4, 4);
    size = le32toh(size);
    // Add the data chunk header
    size += 8;
    return size;
}

data_chunk_t::data_chunk_t(const char *data) {
    // Chunk ID
    memcpy(chunk_ID, data, 4);
    chunk_ID[4] = '\0';
    if (strcmp(chunk_ID, "data") != 0) {
        throw "Error reading data chunk: \"data\" expected";
    }

    // Chunk data size
    memcpy(&chunk_data_size, data + 4, 4);
    chunk_data_size = le32toh(chunk_data_size);
    // Add the header size ("data" & size)
    chunk_data_size += 8;

    // Data
    this->data.d8 = (signed char*) malloc(data_size());
    memcpy(this->data.d8, data + 4, data_size());
}

data_chunk_t::~data_chunk_t() {
    free(data.d8);
}