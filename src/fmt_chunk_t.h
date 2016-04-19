//
// Created by joelm on 2016-04-19.
//

#ifndef CS327_2_0_FMT_CHUNK_H
#define CS327_2_0_FMT_CHUNK_H

class fmt_chunk_t {
public:
    const static int SIZE = 24;
    char chunk_ID[5]; // "fmt "
    unsigned int chunk_data_size; // Always 0x10
    unsigned short compression_code; // Always 0x01
    unsigned short channel_numbers; // 0x01 = mono, 0x02 = stereo
    unsigned int sample_rate; // Hz
    unsigned int bytes_per_second;
    unsigned short bytes_per_sample; // 1 = 8 bit mono, 2 = 8 bit stereo, 2 = 16 bit mono, 4 = 16 bit stereo
    unsigned short bits_per_sample;
public:
    fmt_chunk_t(const char* data);
};

#endif //CS327_2_0_FMT_CHUNK_H
