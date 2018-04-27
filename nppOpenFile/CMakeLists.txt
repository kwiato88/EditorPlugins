cmake_minimum_required(VERSION 2.8)

project(nppOpenFile)

add_definitions(-DUNICODE -D_UNICODE)

add_library(
    nppOpenFile
    SHARED
    Source/dllmain.cpp
    Source/OpenFilePlugin.cpp
    Source/findFile.cpp
    Source/OpenFileDialog.cpp
    # RESOURCES
    Resource/OpenFileDialog.rc
    Resource/OpenFilePluginDetail.rc
    ../WinApi/Resource/SelectPathsDialog.rc
)

target_include_directories(
    nppOpenFile
    PUBLIC
    ../PluginInterface
    ../Utils/Interfaces
    ../Utils/Include
    ../WinApi/Include
    Include
    Include/ExternalInterface
    D:/boost_1_61_0
)

add_dependencies(
    nppOpenFile
    nppUtils
    winApiUtils
)

target_link_libraries(
    nppOpenFile
    nppUtils
    winApiUtils
    comctl32
)

add_library(
    nppOpenFileExternalIf
    STATIC
    Source/OpenFileExternalCommand.cpp
)

target_include_directories(
    nppOpenFileExternalIf
    PUBLIC
    ../PluginInterface
    ../Utils/Include/ExternalInterface
    Include/ExternalInterface
    D:/boost_1_61_0
)

if( MSVC ) # VS2012 doesn't support correctly the tuples yet
    add_definitions( /D _VARIADIC_MAX=10 )
endif()
