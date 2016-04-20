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
    void set_sample(unsigned int index, long value);
    long find_max_sample();
    long max_amplitude();
    unsigned int sample_count();

    void expand(int samples);

    void amplify(float factor);
    void reverse();
    void echo(float weight, unsigned int delay_samples);
    void echo_seconds(float weight, float delay);
    void trim_start(float seconds);
    void trim_end(float seconds);

private:
    unsigned int seconds_to_samples(float seconds);
};

#endif //CS327_2_0_WAV_H
