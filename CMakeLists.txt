cmake_minimum_required(VERSION 3.24.0)

project(FinnAndKaiGame VERSION 0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()


add_subdirectory(FinnKaiGame)

target_link_libraries(FinnAndKaiGame ${CONAN_LIBS})

set_target_properties ( ${PROJECT_NAME}
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/x64/Debug/bin"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/x64/Release/bin"
	VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/x64/Debug/bin"
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/FinnKaiGame/config.xml
	DESTINATION ${CMAKE_BINARY_DIR}/x64/Debug/bin/)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/FinnKaiGame/Assets
	DESTINATION ${CMAKE_BINARY_DIR}/x64/Debug/bin/)