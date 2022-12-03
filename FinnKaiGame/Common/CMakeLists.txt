file (GLOB UTILS_HEADERS "Utils/*.h" "Utils/*.cpp")
file (GLOB COMPONENT_HEADERS "Components/*.h" "Components/*.cpp")
file (GLOB CORE_HEADERS "Core/*.h" "Core/*.cpp")
file (GLOB INTERFACES "Interfaces/*.h" "Interfaces/*.cpp")

add_library(Common STATIC ${CLIENT_SOURCES} ${UTILS_HEADERS} ${CORE_HEADERS} ${COMPONENT_HEADERS} ${INTERFACES})

source_group("Utils" FILES ${UTILS_HEADERS})
source_group("Components" FILES ${COMPONENT_HEADERS})
source_group("Core" FILES ${CORE_HEADERS})
source_group("Interfaces" FILES ${INTERFACES})

target_include_directories(Common
    PUBLIC
        Utils
	  Components
	  Core
	  Interfaces
)
