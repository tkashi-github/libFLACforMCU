cmake_minimum_required(VERSION 3.11)
project(libflac)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DBUILD_TEST -I../source -I../source/include -Wall -Wextra -Wno-long-long -pedantic")

add_executable(libflac
  ./source/buildtest.c
  ./source/bitmath.c
  ./source/bitwriter.c
  ./source/compat.c
  ./source/crc.c
  ./source/float.c
  ./source/md5.c
  ./source/metadata_iterators.c
  ./source/stream_decoder.c
  ./source/stream_encoder_framing.c
  ./source/bitreader.c
  ./source/cpu.c
  ./source/fixed.c
  ./source/format.c
  ./source/lpc.c
  ./source/memory.c
  ./source/metadata_object.c
  ./source/stream_encoder.c
  ./source/window.c
)

