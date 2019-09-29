cmake_minimum_required(VERSION 3.11)
project(mimiclib_unit_test)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUNIT_TEST -I../source -I../cpputest/include -Wall -Wextra -Wno-long-long -pedantic -L../cpputest/lib")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUNIT_TEST -I../source -I../cpputest/include -Wall -Wextra -Wno-long-long -pedantic -L../cpputest/lib")

add_executable(utest
  ./utest/utest.cpp
  ./source/bitmath.c
  ./source/bitreader.c
  ./source/bitwriter.c
  ./source/cpu.c
  ./source/crc.c
  ./source/fixed.c
  ./source/float.c
  ./source/format.c
  ./source/lpc.c
  ./source/md5.c
  ./source/memory.c
  ./source/metadata_iterators.c
  ./source/metadata_object.c
  ./source/stream_decoder.c
  ./source/stream_encoder.c
  ./source/stream_encoder_intrin_sse2.c
  ./source/stream_encoder_intrin_ssse3.c
  ./source/stream_encoder_intrin_avx2.c
  ./source/stream_encoder_framing.c
  ./source/window.c
)

target_link_libraries(utest libCppUTest.a libCppUTestExt.a)
