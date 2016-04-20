//
// Created by joelm on 2016-04-19.
//

#ifndef CS327_2_0_WAV_H
#define CS327_2_0_WAV_H

#include <fstream>
#include <vector>
#include "riff_type_chunk_t.h"
#include "fmt_chunk_t.h"
#include "data_chunk_t.h"

class wav {
public:
    riff_type_chunk_t* riff_type_ptr;
    fmt_chunk_t* fmt_chunk_ptr;
    std::vector<data_chunk_t*> data_chunks;
public:
    wav(std::ifstream& input);
    ~wav();
    void save(const char* file_name);
    long sample(unsigned int index);
    long find_max_sample();
    long max_amplitude();
    unsigned int sample_count();
};

#endif //CS327_2_0_WAV_H
