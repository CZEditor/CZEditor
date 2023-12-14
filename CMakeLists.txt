cmake_minimum_required(VERSION 3.25)
project(CZEditor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui OpenGLWidgets)

qt_standard_project_setup()

set(SOURCES
	src/main.cpp
	src/global.cpp
	src/CzeWindow.cpp
	src/CzeViewport.cpp
	src/CZEditor.cpp
	src/CzeButton.cpp
)

qt_add_executable(czeditor ${SOURCES})

target_link_libraries(czeditor PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui Qt6::OpenGLWidgets opengl32)

