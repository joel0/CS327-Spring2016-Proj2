cmake_minimum_required(VERSION 3.5)
project(CS327_2_0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp src/wav.cpp src/wav.h src/riff_type_chunk_t.cpp src/riff_type_chunk_t.h src/fmt_chunk_t.cpp src/fmt_chunk_t.h src/data_chunk_t.cpp src/data_chunk_t.h)
add_executable(CS327_2_0 ${SOURCE_FILES})