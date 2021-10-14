cmake_minimum_required(VERSION 3.17)
project(lab1)
set(CMAKE_CXX_STANDARD 17)
add_subdirectory(lib)

set(SRCS src/main.cpp src/config.cpp src/utils.cpp src/bodyFallMathModel.cpp)
set(LIBS euler rungeKutta)

include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/lib/include)

target_include_directories(euler PUBLIC ${CMAKE_SOURCE_DIR}/lib/include)
target_include_directories(rungeKutta PUBLIC ${CMAKE_SOURCE_DIR}/lib/include)

add_executable(lab1 ${SRCS})
target_link_libraries(lab1 ${LIBS} ${CMAKE_DL_LIBS})

configure_file(config.cfg config.cfg COPYONLY)