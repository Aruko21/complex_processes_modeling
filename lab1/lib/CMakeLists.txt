cmake_minimum_required(VERSION 3.17)
project(libs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/lib/include)

add_library(euler SHARED ${CMAKE_SOURCE_DIR}/lib/src/euler.cpp)
add_library(rungeKutta SHARED ${CMAKE_SOURCE_DIR}/lib/src/rungeKutta.cpp)

target_sources(euler PRIVATE ${CMAKE_SOURCE_DIR}/src/bodyFallMathModel.cpp ${CMAKE_SOURCE_DIR}/src/config.cpp)
target_sources(rungeKutta PRIVATE ${CMAKE_SOURCE_DIR}/src/bodyFallMathModel.cpp ${CMAKE_SOURCE_DIR}/src/config.cpp)