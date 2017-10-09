cmake_minimum_required(VERSION 2.8)

project(nppUtils)

add_definitions(-DUNICODE -D_UNICODE)

add_library(
    nppUtils
    STATIC
    Source/ShellCommand.cpp
    Source/EditorData.cpp
    Source/Logger.cpp
    Source/Log.cpp
    Source/NppTextReader.cpp
    Source/NppMessagePrinter.cpp
    Source/NppLocationGetter.cpp
    Source/NppLocationSetter.cpp
    Source/NppListViewSelector.cpp
    Source/NppPathGetter.cpp
    Source/NppPathsSelector.cpp
)

target_include_directories(
    nppUtils
    PUBLIC
    ../PluginInterface
    Interfaces
    Include
    ../WinApi/Include
    D:/boost_1_61_0
)

if( MSVC ) # VS2012 doesn't support correctly the tuples yet
    add_definitions( /D _VARIADIC_MAX=10 )
endif()
