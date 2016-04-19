//
// Created by joelm on 2016-04-19.
//

#ifndef CS327_2_0_RIFF_TYPE_CHUNK_H
#define CS327_2_0_RIFF_TYPE_CHUNK_H

class riff_type_chunk_t {
public:
    const static int SIZE = 12; // RIFF header, file size, and "WAVE"
    char chunk_ID[5]; // "RIFF"
    unsigned int file_size; // Chunk Data Size + 8
    char riff_type[5]; // "WAVE"

public:
    riff_type_chunk_t(const char* data);
    char* file_data();
};

#endif //CS327_2_0_RIFF_TYPE_CHUNK_H
