cmake_minimum_required(VERSION 3.25)
project(CZEditor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui OpenGLWidgets)

qt_standard_project_setup()

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

qt_add_executable(czeditor ${SOURCES})

if(WIN32)
	target_link_libraries(czeditor PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui Qt6::OpenGLWidgets opengl32)
elseif(UNIX)
	target_link_libraries(czeditor PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui Qt6::OpenGLWidgets GL)
endif()
