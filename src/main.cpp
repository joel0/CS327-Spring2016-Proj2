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
    std::cout << w->fmt_chunk_ptr->bits_per_sample << std::endl;
    std::cout << w->fmt_chunk_ptr->bytes_per_sample << std::endl;
    std::cout << "max amp: " << w->max_amplitude() << std::endl;
    std::cout << "Sample count: " << w->sample_count() << std::endl;
    std::cout << "Max sample: " << w->find_max_sample() << std::endl;

    std::cout << std::endl;
    for (unsigned int i = 0; i < 800; i++) {
        printf("%d ", (int) w->sample(i));
    }
    std::cout << std::endl;

    //w->amplify(0.5);
    w->reverse();
    //w->fmt_chunk_ptr->sample_rate *= 2;
    //w->fmt_chunk_ptr->bytes_per_second *= 2;

    try {
        w->save("c:\\Users\\joelm\\Downloads\\11k16bitpcm-2.wav");
        std::cout << "Saved the file" << std::endl;
    } catch (const char* ex) {
        std::cout << ex << std::endl;
    }

    return 0;
}