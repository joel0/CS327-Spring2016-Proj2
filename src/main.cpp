#include <iostream>
#include <fstream>
#include <cstring>
#include "wav.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: wav in_file.wav out_file.wav [arguments]" << std::endl;
        std::cout << "\tAny number of arguments may be used in combination with each other." << std::endl;
        std::cout << "\tThe arguments are applied left-to-right cumulatively.  The argument" << std::endl;
        std::cout << "\tparsing is delicate, so please be careful to use the arguments properly." << std::endl;
        std::cout << "Arguments:" << std::endl;
        std::cout << "\t-r\t\tReverses the audio file" << std::endl;
        std::cout << "\t-a (f)\t\tAmplifies the audio by the given ratio as a float" << std::endl;
        std::cout << "\t-e (f) (f)\tAdds an echo effect with the amplitude and delay" << std::endl;
        std::cout << "\t-ts (f)\t\tTrims the start of the file" << std::endl;
        std::cout << "\t-te (f)\t\tTrims the end of the file" << std::endl;
        return 0;
    }

    std::ifstream in(argv[1], ios::in);
    wav* w;
    std::cout << "Loading audio file..." << std::endl;
    try {
        w = new wav(in);
    } catch (const char* ex) {
        std::cout << ex << std::endl;
        return -1;
    }
    in.close();
    std::cout << std::endl;
    std::cout << "Info:" << std::endl;
    std::cout << "Sample rate: " << w->fmt_chunk_ptr->sample_rate << std::endl;
    std::cout << "Bits per sample: " << w->fmt_chunk_ptr->bits_per_sample << std::endl;
    std::cout << std::endl;
    std::cout << "Processing:" << std::endl;

    for (int i = 3; i < argc; i++) {
        try {
            if (strcmp(argv[i], "-r") == 0) {
                std::cout << "Reversing..." << std::endl;
                w->reverse();
            } else if (strcmp(argv[i], "-a") == 0) {
                if (++i == argc) {
                    std::cout << "Argument required for -a" << std::endl;
                    return -1;
                }
                std::cout << "Amplifying..." << std::endl;
                w->amplify((float) atof(argv[i]));
            } else if (strcmp(argv[i], "-e") == 0) {
                if (++i == argc) {
                    std::cout << "Arguments required for -e" << std::endl;
                    return -1;
                }
                float weight = (float) atof(argv[i]);
                if (++i == argc) {
                    std::cout << "Two arguments required for -e" << std::endl;
                    return -1;
                }
                std::cout << "Adding echo..." << std::endl;
                w->echo_seconds(weight, (float) atof(argv[i]));
            } else if (strcmp(argv[i], "-ts") == 0) {
                if (++i == argc) {
                    std::cout << "Argument required for -ts" << std::endl;
                    return -1;
                }
                std::cout << "Trimming start..." << std::endl;
                w->trim_start((float) atof(argv[i]));
            } else if (strcmp(argv[i], "-te") == 0) {
                if (++i == argc) {
                    std::cout << "Argument required for -te" << std::endl;
                    return -1;
                }
                std::cout << "Trimming end..." << std::endl;
                w->trim_end((float) atof(argv[i]));
            } else {
                std::cout << "Unknown argument: " << argv[i] << std::endl;
            }
        } catch (const char* ex) {
            std::cout << ex << std::endl;
        }
    }

    std::cout << std::endl;
    std::cout << "Saving..." << std::endl;
    try {
        w->save(argv[2]);
        std::cout << "Saved" << std::endl;
    } catch (const char* ex) {
        std::cout << ex << std::endl;
        delete w;
        return -1;
    }
    delete w;

    return 0;
}