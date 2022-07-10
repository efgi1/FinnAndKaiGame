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
