cmake_minimum_required(VERSION 3.20)
project(MonteCarloEngine VERSION 1.0.0 LANGUAGES CXX)

# Use Modern C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags for high-performance simulations
if(MSVC)
    add_compile_options(/W4 /WX /Ot /arch:AVX2)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -O3 -march=native)
endif()

# Directories
include_directories(include)

# Executable (we will add source files as we create them)
add_executable(mc_engine src/main.cpp src/RandomGenerator.cpp src/PathGenerator.cpp)
