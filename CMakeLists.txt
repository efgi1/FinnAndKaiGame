cmake_minimum_required(VERSION 3.24.0)

project(FinnAndKaiGame VERSION 0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()


add_subdirectory(FinnKaiGame)

target_link_libraries(FinnAndKaiGame ${CONAN_LIBS})