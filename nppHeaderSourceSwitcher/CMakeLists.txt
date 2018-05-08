cmake_minimum_required(VERSION 2.8)

project(nppHeaderSourceSwitcher)

add_definitions(-DUNICODE -D_UNICODE)

add_library(
    nppHeaderSourceSwitcher
    SHARED
    Source/dllmain.cpp
    Source/SwitcherPlugin.cpp
    Source/SwitcherFunction.cpp
    Source/SwitchFile.cpp
    Source/SavelLoadSwitchInfo.cpp
    # RESOURCES
    Resource/HeaderSourcePluginDetail.rc
    ../WinApi/Resource/GridDialog.rc
    ../WinApi/Resource/QueryDialog.rc
)

target_include_directories(
    nppHeaderSourceSwitcher
    PUBLIC
    ../PluginInterface
    ../Utils/Interfaces
    ../Utils/Include
    ../WinApi/Include
    Include
    D:/boost_1_61_0
)

add_dependencies(
    nppHeaderSourceSwitcher
    nppUtils
    winApiUtils
)

target_link_libraries(
    nppHeaderSourceSwitcher
    nppUtils
    winApiUtils
    comctl32
)

add_executable(
    HeaderSourceSwitchTests
    Source/SwitchFile.cpp
    Source/SavelLoadSwitchInfo.cpp
    Tests/SwitchFileTS.cpp
    Tests/ValidateTS.cpp
    Tests/SaveLoadTS.cpp
)

target_include_directories(
    HeaderSourceSwitchTests
    PUBLIC
    ../Utils/Interfaces
    Include
    D:/boost_1_61_0
    D:/googletest-release-1.6.0/include
    D:/googlemock-release-1.6.0/include
)

target_link_libraries(
    HeaderSourceSwitchTests
    PUBLIC
    gtest.lib
    gmock.lib
    gtest_main.lib
)

if( MSVC ) # VS2012 doesn't support correctly the tuples yet
    add_definitions( /D _VARIADIC_MAX=10 )
endif()
