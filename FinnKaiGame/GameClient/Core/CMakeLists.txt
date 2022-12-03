file (GLOB ENGINE_HEADERS "Engine/*.h" "Engine/*.cpp")
file (GLOB FRAMEWORK_HEADERS "Framework/*.h" "Framework/*.cpp")
file (GLOB MANAGERS_HEADERS "Managers/*.h" "Managers/*.cpp")


target_sources(${PROJECT_NAME}Client
    PRIVATE
	  ${ENGINE_HEADERS}
	  ${FRAMEWORK_HEADERS}
	  ${MANAGERS_HEADERS}
)

target_include_directories(${PROJECT_NAME}Client
    PRIVATE
	  Engine
	  Framework
	  Managers
)
