cmake_minimum_required(VERSION 3.25)
project(CZEditor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(Qt6GuiTools_DIR "D:/a/_temp/msys64/mingw64/qt6-static/lib/cmake/Qt6GuiTools")
set(CMAKE_DISABLE_FIND_PACKAGE_harfbuzz TRUE)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a;.lib")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ D:/a/_temp/msys64/mingw64/lib/libmng.a D:/a/_temp/msys64/usr/lib/libbrotlidec.a D:/a/_temp/msys64/usr/lib/libbz2.a")
set(VERBOSE 1)
set(HAVE_GETTIME ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui OpenGLWidgets)

qt_standard_project_setup()

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

qt_add_executable(czeditor ${SOURCES})

if(WIN32)
	target_link_libraries(czeditor PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui Qt6::OpenGLWidgets opengl32 b2 bz2 brotlidec harfbuzz freetype jpeg graphite2 mng freetype png pcre2-16 tiff jbig webp lerc lzma zstd deflate sharpyuv Rpcrt4.lib -static)
elseif(UNIX)
	target_link_libraries(czeditor PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui Qt6::OpenGLWidgets GL -static)
endif()

