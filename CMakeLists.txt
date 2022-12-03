cmake_minimum_required(VERSION 3.24.0)

project(FinnKaiGame VERSION 0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(${CMAKE_BINARY_DIR}/conanbuildinfo_multi.cmake)
conan_basic_setup()


add_subdirectory(${PROJECT_NAME})


foreach(_LIB ${CONAN_LIBS_RELEASE})
	target_link_libraries(Common optimized ${_LIB})
	target_link_libraries(${PROJECT_NAME}Client optimized ${_LIB})
	target_link_libraries(${PROJECT_NAME}Server optimized ${_LIB})
endforeach()
foreach(_LIB ${CONAN_LIBS_DEBUG})
	target_link_libraries(Common optimized ${_LIB})
	target_link_libraries(${PROJECT_NAME}Client debug ${_LIB})
	target_link_libraries(${PROJECT_NAME}Server debug ${_LIB})
endforeach()

set_target_properties ( ${PROJECT_NAME}Client
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/x64/Debug/bin"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/x64/Release/bin"
	VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/x64/Debug/bin"
)

set_target_properties ( ${PROJECT_NAME}Server
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/x64/Debug/bin"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/x64/Release/bin"
	VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/x64/Debug/bin"
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/FinnKaiGame/config.xml
	DESTINATION ${CMAKE_BINARY_DIR}/x64/Debug/bin/)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/FinnKaiGame/Assets
	DESTINATION ${CMAKE_BINARY_DIR}/x64/Debug/bin/)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/FinnKaiGame/config.xml
	DESTINATION ${CMAKE_BINARY_DIR}/x64/Release/bin/)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/FinnKaiGame/Assets
	DESTINATION ${CMAKE_BINARY_DIR}/x64/Release/bin/)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME}Client)