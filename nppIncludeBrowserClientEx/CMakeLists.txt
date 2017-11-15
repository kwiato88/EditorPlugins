cmake_minimum_required(VERSION 2.8)

project(nppIncludeBrowserClientEx)

add_definitions(-DUNICODE -D_UNICODE)

link_directories(
    D:/boost_1_61_0/stage/lib
    D:/test/EditorPlugins/nppIncludeBrowserClientEx/ExternalDeps
)

add_library(
    nppIncludeBrowserClientEx
    SHARED
    Source/dllmain.cpp
    Source/Plugin.cpp
)

target_include_directories(
    nppIncludeBrowserClientEx
    PUBLIC
    ExternalDeps/Include
    Include
    D:/boost_1_61_0
)

target_link_libraries(
    nppIncludeBrowserClientEx
    nppIncludeBrowserExternalIf.lib
)

if( MSVC ) # VS2012 doesn't support correctly the tuples yet
    add_definitions( /D _VARIADIC_MAX=10 )
endif()
