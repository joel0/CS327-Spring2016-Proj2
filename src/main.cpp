#include <iostream>
#include <fstream>
#include "wav.h"

using namespace std;

int main() {
    std::ifstream in("c:\\Users\\joelm\\Downloads\\11k16bitpcm.wav", ios::in);
    wav* w;
    try {
        w = new wav(in);
    } catch (const char* ex) {
        std::cout << ex << std::endl;
    }
    in.close();
    std::cout << w->riff_type_ptr->file_size << std::endl;
    std::cout << w->fmt_chunk_ptr->sample_rate << std::endl;
    std::cout << w->data_chunks.size() << std::endl;
    return 0;
}