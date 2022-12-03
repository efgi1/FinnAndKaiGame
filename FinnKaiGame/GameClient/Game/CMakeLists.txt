file (GLOB COMPONENTS_SOURCES "Components/*.h" "Components/*.cpp")
file (GLOB SCENES_SOURCES "Scenes/*.h" "Scenes/*.cpp")
file (GLOB SYSTEMS_SOURCES "Systems/*.h" "Systems/*.cpp")

target_sources(${PROJECT_NAME}Client
    PRIVATE
	  ${COMPONENTS_SOURCES}
	  ${SCENES_SOURCES}
	  ${SYSTEMS_SOURCES}
)

target_include_directories(${PROJECT_NAME}Client
    PRIVATE
	  Components
	  Scenes
	  Systems
)