cmake_minimum_required(VERSION 3.26)
project(GetSmaler)

set(CMAKE_CXX_STANDARD 23)


add_executable(GetSmaler main.cpp
        Reader.cpp
        Reader.h
        Writer.cpp
        Writer.h
        decode.h
        decode.cpp
        encode.cpp
        encode.h)
