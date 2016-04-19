//
// Created by joelm on 2016-04-19.
//

#include <cstring>
#include <endian.h>
#include "riff_type_chunk_t.h"
#include "fmt_chunk_t.h"

riff_type_chunk_t::riff_type_chunk_t(const char *data) {
    // Chunk ID
    memcpy(chunk_ID, data, 4);
    chunk_ID[4] = '\0';
    if (strcmp(chunk_ID, "RIFF") != 0) {
        throw "RIFF type chunk ID is incorrect (should be \"RIFF\")";
    }

    // File size (stored as bytes remaining after the size value)
    memcpy(&file_size, data + 4, 4);
    file_size = le32toh(file_size);
    // Add 8 bytes for "RIFF" & file size (32-bits)
    file_size += 8;
    if (file_size < fmt_chunk_t::SIZE + SIZE + 8) {
        throw "RIFF type chunk file size is too small to contain the fmt chunk and data";
    }

    // RIFF Type
    memcpy(riff_type, data + 8, 4);
    riff_type[4] = '\0';
    if (strcmp(riff_type, "WAVE") != 0) {
        throw "RIFF type is incorrect (should be \"WAVE\")";
    }
}