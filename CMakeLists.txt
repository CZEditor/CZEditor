cmake_minimum_required(VERSION 3.25)
project(CZEditor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

qt_standard_project_setup()

set(SOURCES
	main.cpp
	global.cpp
	CzeWindow.cpp
	CzeViewport.cpp
	CZEditor.cpp
	CzeButton.cpp
)

set(RESOURCES
	CZEditor.qrc
)

qt_add_executable(czeditor ${SOURCES} ${RESOURCES})

target_link_libraries(czeditor PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui)

