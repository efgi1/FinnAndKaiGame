file (GLOB CORE_SOURCES "Core/*.cpp" "Core/*.h")
file (GLOB SOURCE "src/*.cpp")

add_executable(${PROJECT_NAME}Server ${SOURCE} ${CORE_SOURCES})
target_link_libraries(${PROJECT_NAME}Server Common)

source_group("Core" FILES ${CORE_SOURCES})
source_group("src" FILES ${SOURCE})

target_include_directories(${PROJECT_NAME}Server
    PRIVATE
        Core
)