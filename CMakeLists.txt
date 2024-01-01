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
if(WIN32)
	find_library(libavutil NAMES "avutil" HINTS "${CMAKE_SOURCE_DIR}/lib/libav" REQUIRED)
	find_library(libavformat NAMES "avformat" HINTS "${CMAKE_SOURCE_DIR}/lib/libav" REQUIRED)
	find_library(libavdevice NAMES "avdevice" HINTS "${CMAKE_SOURCE_DIR}/lib/libav" REQUIRED)
	find_library(libavcodec NAMES "avcodec" HINTS "${CMAKE_SOURCE_DIR}/lib/libav" REQUIRED)
	target_include_directories(${PROJECT_NAME} AFTER PRIVATE "${CMAKE_SOURCE_DIR}/lib/libav/include")
	configure_file("${CMAKE_SOURCE_DIR}/lib/libav/avutil-58.dll" "${CMAKE_CURRENT_BINARY_DIR}/avutil-58.dll" COPYONLY)
	configure_file("${CMAKE_SOURCE_DIR}/lib/libav/avformat-60.dll" "${CMAKE_CURRENT_BINARY_DIR}/avformat-60.dll" COPYONLY)
	configure_file("${CMAKE_SOURCE_DIR}/lib/libav/avdevice-60.dll" "${CMAKE_CURRENT_BINARY_DIR}/avdevice-60.dll" COPYONLY)
	configure_file("${CMAKE_SOURCE_DIR}/lib/libav/swresample-4.dll" "${CMAKE_CURRENT_BINARY_DIR}/swresample-4.dll" COPYONLY)
endif()
# Link libraries

target_link_libraries(${PROJECT_NAME} PRIVATE
	Qt6::Core
	Qt6::Widgets
	Qt6::Gui
	Qt6::OpenGLWidgets
 	OpenGL::GL
	${libavutil}
	${libavformat}
	${libavdevice}
	${libavcodec}
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

