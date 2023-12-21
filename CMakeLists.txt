cmake_minimum_required(VERSION 3.25)
project(czeditor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(VERBOSE 1)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui OpenGLWidgets)

qt_standard_project_setup()

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

qt_add_executable(${PROJECT_NAME} ${SOURCES})

if(WIN32)
	target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui Qt6::OpenGLWidgets opengl32)
elseif(UNIX)
	target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui Qt6::OpenGLWidgets GL)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
 	target_compile_options(${PROJECT_NAME} PRIVATE -stdlib=libc++)
endif()

install(TARGETS ${PROJECT_NAME})
