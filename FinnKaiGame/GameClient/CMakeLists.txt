file(GLOB SOURCES "src/*.cpp" )
file (GLOB IMGUI_SOURCES "imgui/*.cpp")

add_executable(${PROJECT_NAME}Client ${SOURCES} ${IMGUI_SOURCES})

add_subdirectory(Core)
add_subdirectory(Game)

target_link_libraries(${PROJECT_NAME}Client Common)

file (GLOB_RECURSE ALL "*.cpp" "*.h")
source_group(TREE "C:/Users/nateh/Desktop/Projects/FinnKaiGame/FinnKaiGame/GameClient" FILES ${ALL})

target_include_directories(${PROJECT_NAME}Client
    PRIVATE
	  imgui
)
