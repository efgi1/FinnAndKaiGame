file(GLOB CLIENT_SOURCES
	"GameClient/*.cpp"
	"GameClient/*.h"
)

file(GLOB SERVER_SOURCES
	"GameServer/*.cpp"
	"GameServer/*.h"
)

file (GLOB COMMON_HEADERS "Common/*.h" "Common/*.cpp")
file (GLOB COMPONENT_HEADERS "Components/*.h" "Components/*.cpp")
file (GLOB CORE_HEADERS "Core/*.h" "Core/*.cpp")
file (GLOB SYSTEM_HEADERS "Systems/*.h" "Systems/*.cpp")
file (GLOB IMGUI "imgui/*.h" "imgui/*.cpp")

add_executable(${PROJECT_NAME}Client ${CLIENT_SOURCES} ${COMMON_HEADERS} ${CORE_HEADERS} ${COMPONENT_HEADERS} ${SYSTEM_HEADERS} ${IMGUI})
add_executable(${PROJECT_NAME}Server ${SERVER_SOURCES} ${COMMON_HEADERS} ${CORE_HEADERS} ${COMPONENT_HEADERS} ${SYSTEM_HEADERS} ${IMGUI})

source_group("Common" FILES ${COMMON_HEADERS})
source_group("Components" FILES ${COMPONENT_HEADERS})
source_group("Core" FILES ${CORE_HEADERS})
source_group("System" FILES ${SYSTEM_HEADERS})
source_group("imgui" FILES ${IMGUI})
source_group("Client" FILES ${CLIENT_SOURCES})
source_group("Server" FILES ${SERVER_SOURCES})

target_include_directories(${PROJECT_NAME}Client
    PRIVATE
        Common
	  Components
	  Core
	  imgui
	  Systems
	  GameClient
)


target_include_directories(${PROJECT_NAME}Server
    PRIVATE
        Common
	  Components
	  Core
	  imgui
	  Systems
	  GameServer
)
