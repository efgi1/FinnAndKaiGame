file(GLOB SOURCES
	"*.cpp"
)

file(GLOB TOP_HEADERS "*.h")

file (GLOB COMMON_HEADERS "Common/*.h" "Common/*.cpp")
file (GLOB CORE_HEADERS "Core/*.h" "Core/*.cpp")
file (GLOB COMPONENT_HEADERS "Components/*.h" "Components/*.cpp")
file (GLOB SYSTEM_HEADERS "Systems/*.h" "Systems/*.cpp")
file (GLOB IMGUI "imgui/*.h" "imgui/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES} ${TOP_HEADERS} ${COMMON_HEADERS} ${CORE_HEADERS} ${COMPONENT_HEADERS} ${SYSTEM_HEADERS} ${IMGUI})

source_group("Common" FILES ${COMMON_HEADERS})
source_group("Core" FILES ${CORE_HEADERS})
source_group("Components" FILES ${COMPONENT_HEADERS})
source_group("System" FILES ${SYSTEM_HEADERS})
source_group("imgui" FILES ${IMGUI})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        Common
	  Components
	  Core
	  imgui
	  Systems
	  ./
)
