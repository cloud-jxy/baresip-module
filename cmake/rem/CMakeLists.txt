cmake_minimum_required(VERSION 3.4.1)

project(rem)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -ggdb")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_INTTYPES_H")

set(source_list
    ../../src/main/cpp/rem/src/fir/fir.c
    ../../src/main/cpp/rem/src/goertzel/goertzel.c
    ../../src/main/cpp/rem/src/g711/g711.c
    ../../src/main/cpp/rem/src/aubuf/aubuf.c
    ../../src/main/cpp/rem/src/aufile/aufile.c
    ../../src/main/cpp/rem/src/aufile/wave.c
    ../../src/main/cpp/rem/src/auresamp/resamp.c
    ../../src/main/cpp/rem/src/autone/tone.c
    ../../src/main/cpp/rem/src/dtmf/dec.c
    ../../src/main/cpp/rem/src/au/fmt.c
    ../../src/main/cpp/rem/src/auconv/auconv.c
    ../../src/main/cpp/rem/src/vid/fmt.c
    ../../src/main/cpp/rem/src/vid/frame.c
    ../../src/main/cpp/rem/src/vid/draw.c
    ../../src/main/cpp/rem/src/vidconv/vconv.c
)

include_directories(../../src/main/cpp/re/include)
include_directories(../../src/main/cpp/rem/include)
add_library(rem STATIC ${source_list})
