file(GLOB SOURCES
	"*.cpp"
	"Common/*.cpp"
	"Components/*.cpp"
    	"Core/*.cpp"
    	"imgui/*.cpp"
    	"Systems/*.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        Common
	  Components
	  Core
	  imgui
	  Systems
	  ./
)

#[[
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy directory
                ${CMAKE_SOURCE_DIR}/Assets
                ${CMAKE_CURRENT_BINARY_DIR}/../bin)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/config.txt
                ${CMAKE_CURRENT_BINARY_DIR}/bin/config.txt)
]]