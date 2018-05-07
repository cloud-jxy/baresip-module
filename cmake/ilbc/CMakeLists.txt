cmake_minimum_required(VERSION 3.4.1)

project(ilbc)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -ggdb")
set(source_list
    ../../src/main/cpp/libilbc/src/anaFilter.c
    ../../src/main/cpp/libilbc/src/constants.c
    ../../src/main/cpp/libilbc/src/createCB.c
    ../../src/main/cpp/libilbc/src/doCPLC.c
    ../../src/main/cpp/libilbc/src/enhancer.c
    ../../src/main/cpp/libilbc/src/filter.c
    ../../src/main/cpp/libilbc/src/FrameClassify.c
    ../../src/main/cpp/libilbc/src/gainquant.c
    ../../src/main/cpp/libilbc/src/getCBvec.c
    ../../src/main/cpp/libilbc/src/helpfun.c
    ../../src/main/cpp/libilbc/src/hpInput.c
    ../../src/main/cpp/libilbc/src/hpOutput.c
    ../../src/main/cpp/libilbc/src/iCBConstruct.c
    ../../src/main/cpp/libilbc/src/iCBSearch.c
    ../../src/main/cpp/libilbc/src/iLBC_decode.c
    ../../src/main/cpp/libilbc/src/iLBC_encode.c
    ../../src/main/cpp/libilbc/src/LPCdecode.c
    ../../src/main/cpp/libilbc/src/LPCencode.c
    ../../src/main/cpp/libilbc/src/lsf.c
    ../../src/main/cpp/libilbc/src/packing.c
    ../../src/main/cpp/libilbc/src/StateConstructW.c
    ../../src/main/cpp/libilbc/src/StateSearchW.c
    ../../src/main/cpp/libilbc/src/syntFilter.c
)

include_directories(../../src/main/cpp/libilbc/src)

add_library(ilbc STATIC ${source_list})
