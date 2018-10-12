cmake_minimum_required(VERSION 3.12)
project(cpp_pic_proc_mr3317)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(cpp_pic_proc_mr3317
        images/ducks1.jpg
        images/ducks2.jpg
        images/ducks3.jpg
        images/keepcalm.png
        images/test.jpg
        Colour.cpp
        Colour.hpp
        example_input.txt
        Main.cpp
        PicLibrary.cpp
        PicLibrary.hpp
        Picture.cpp
        Picture.hpp
        Utils.cpp
        Utils.hpp)
