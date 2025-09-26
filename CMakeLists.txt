cmake_minimum_required(VERSION 3.5)
project(SPIDevTest)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(third_party)

add_executable(SPIDevTest src/main.cpp)

target_link_libraries(SPIDevTest SpidevMatrix)
