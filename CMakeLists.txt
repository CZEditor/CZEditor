cmake_minimum_required(VERSION 3.25)
project(CZEditor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Dependencies

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui OpenGLWidgets)
find_package(OpenGL REQUIRED)

qt_standard_project_setup()

# Automatically find C++ source files in the src/ directory

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

qt_add_executable(${PROJECT_NAME} ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC TRUE)

# Platform-dependent setup

# Hide console on Windows Release builds
if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
	target_link_options(${PROJECT_NAME} PRIVATE /SUBSYSTEM:windows /ENTRY:mainCRTStartup)
endif()

# Link libraries

target_link_libraries(${PROJECT_NAME} PRIVATE
	Qt6::Core
	Qt6::Widgets
	Qt6::Gui
	Qt6::OpenGLWidgets
 	OpenGL::GL
)

# Link and compile with libc++ instead of libstdc++ if on Clang

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
 	target_compile_options(${PROJECT_NAME} PRIVATE -stdlib=libc++)
 	target_link_options(${PROJECT_NAME} PRIVATE -stdlib=libc++)
endif()

# Configure installation

install(TARGETS ${PROJECT_NAME})

if(UNIX)
	install(FILES data/desktop/CZEditor.desktop DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications")
	install(FILES data/desktop/CZEditor.png DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/pixmap")
endif()

