cmake_minimum_required(VERSION 2.8)

project(nppCTagsClientEx)

add_definitions(-DUNICODE -D_UNICODE)

link_directories(
    D:/boost_1_61_0/stage/lib
    D:/test/EditorPlugins/nppCtagsClientEx/ExternalDeps
)

add_library(
    nppCTagsClientEx
    SHARED
    Source/dllmain.cpp
    Source/Plugin.cpp
)

target_include_directories(
    nppCTagsClientEx
    PUBLIC
    ExternalDeps/Include
    Include
    D:/boost_1_61_0
)

target_link_libraries(
    nppCTagsClientEx
    nppCTagsExternalIf.lib
)

if( MSVC ) # VS2012 doesn't support correctly the tuples yet
    add_definitions( /D _VARIADIC_MAX=10 )
endif()
