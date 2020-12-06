cmake_minimum_required(VERSION 3.15)
project(Hashtable VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(
    GLOB SOURCES
    main.cpp
    #LinearProb/LinearHT.cpp
    #QuadrProb/QuadrHT.cpp
)

#add_subdirectory(DoubleHashing)
#add_subdirectory(Cuckoo)
#add_subdirectory(Chaining)

add_executable(benchmarks ${SOURCES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -O3 -fsanitize=address -fsanitize=undefined -fstack-protector")