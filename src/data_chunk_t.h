//
// Created by joelm on 2016-04-19.
//

#ifndef CS327_2_0_DATA_CHUNK_H
#define CS327_2_0_DATA_CHUNK_H

class data_chunk_t {
public:
    char chunk_ID[5]; // "data"
    unsigned int chunk_data_size;
    char* data;
public:
    data_chunk_t(const char* data);
    ~data_chunk_t();
    static unsigned int read_chunk_size(char* data);
};

#endif //CS327_2_0_DATA_CHUNK_H
