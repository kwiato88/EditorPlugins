cmake_minimum_required(VERSION 2.8)

project(nppCTagPlugin)

add_definitions(-DUNICODE -D_UNICODE)

add_library(
    ProjectMgmt
    STATIC
    Source/Project.cpp
    Source/Workspace.cpp
    Source/SwitchableWorkspace.cpp
)

target_include_directories(
    ProjectMgmt
    PUBLIC
    ../Utils/Interfaces
    ../Utils/Include
    Include
    D:/boost_1_61_0
)

add_library(
    nppProjectPlugin
    SHARED
    Source/dllmain.cpp
    Source/Plugin.cpp
    Source/CTags.cpp
    Source/NppIncludes.cpp
    Source/NppFiles.cpp
    Resource/ProjectPluginDetail.rc
    ../WinApi/Resource/GridDialog.rc
    ../WinApi/Resource/QueryDialog.rc
)

target_include_directories(
    nppProjectPlugin
    PUBLIC
    ../PluginInterface
    ../Utils/Interfaces
    ../Utils/Include
    ../WinApi/Include
    ../nppCtagPlugin/Include/ExternalInterface
    ../nppIncludeBrowser/Include/ExternalInterface
    ../nppOpenFile/Include/ExternalInterface
    Include
    D:/boost_1_61_0
)

add_dependencies(
    nppProjectPlugin
    ProjectMgmt
    nppUtils
    winApiUtils
    nppCTagsExternalIf
    nppIncludeBrowserExternalIf
    nppOpenFileExternalIf
)

target_link_libraries(
    nppProjectPlugin
    ProjectMgmt
    nppUtils
    winApiUtils
    nppCTagsExternalIf
    nppIncludeBrowserExternalIf
    nppOpenFileExternalIf
    comctl32
)

add_executable(
    ProjectMT
    Tests/ProjectMT.cpp
)

target_include_directories(
    ProjectMT
    PUBLIC
    ../Utils/Interfaces
    ../Utils/Include
    ../Utils/Tests
    Include
    D:/boost_1_61_0
    D:/googletest-release-1.6.0/include
    D:/googlemock-release-1.6.0/include
)

add_dependencies(
    ProjectMT
    ProjectMgmt
)

target_link_libraries(
    ProjectMT
    ProjectMgmt
    D:/googletest-release-1.6.0/Build/Release/gtest.lib
    D:/googlemock-release-1.6.0/Build/Release/gmock.lib
    D:/googlemock-release-1.6.0/Build/Release/gmock_main.lib
)

if( MSVC ) # VS2012 doesn't support correctly the tuples yet
    add_definitions( /D _VARIADIC_MAX=10 )
endif()
