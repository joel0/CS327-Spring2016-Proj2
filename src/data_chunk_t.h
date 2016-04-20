//
// Created by joelm on 2016-04-19.
//

#ifndef CS327_2_0_DATA_CHUNK_H
#define CS327_2_0_DATA_CHUNK_H

typedef union {
    signed char* d8;
    short* d16;
    int* d32;
    long* d64;
} data_t;

class data_chunk_t {
public:
    char chunk_ID[5]; // "data"
    unsigned int chunk_data_size;
    data_t data;
public:
    data_chunk_t(const char* data);
    ~data_chunk_t();
    inline unsigned int data_size() { return chunk_data_size - 8; };
    char* file_data();
    static unsigned int read_chunk_size(char* data);
};

#endif //CS327_2_0_DATA_CHUNK_H
