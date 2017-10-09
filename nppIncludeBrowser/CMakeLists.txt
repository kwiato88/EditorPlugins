cmake_minimum_required(VERSION 2.8)

project(nppIncludeBrowser)

add_definitions(-DUNICODE -D_UNICODE)

add_library(
    nppIncludeBrowser
    SHARED
    Source/dllmain.cpp
    Source/IncludeBrowserPlugin.cpp
    Source/FindCppInclude.cpp
    Source/FindInclude.cpp
    Source/FindTtcnInclude.cpp
    Source/IncludeBrowser.cpp
    Source/MutilpeFilesIncludeBrowser.cpp
    Source/IncludeBrowserController.cpp
    # RESOURCES
    ../WinApi/Resource/ListBoxDialog.rc
    Resource/IncludeBrowserPluginDetail.rc
)

target_include_directories(
    nppIncludeBrowser
    PUBLIC
    ../PluginInterface
    ../Utils/Interfaces
    ../Utils/Include
    ../WinApi/Include
    Include
    D:/boost_1_61_0
)

target_link_libraries(
    nppIncludeBrowser
    nppUtils
    winApiUtils
)

add_executable(
    IncludeBrowserTests
    Source/FindCppInclude.cpp
    Source/FindInclude.cpp
    Source/FindTtcnInclude.cpp
    Tests/FindIncludeTS.cpp
)

target_include_directories(
    IncludeBrowserTests
    PUBLIC
    ../Utils/Interfaces
    Include
    D:/boost_1_61_0
    D:/googletest-release-1.6.0/include
    D:/googlemock-release-1.6.0/include
)

target_link_libraries(
    IncludeBrowserTests
    gtest.lib
    gmock.lib
    gtest_main.lib
)

if( MSVC ) # VS2012 doesn't support correctly the tuples yet
    add_definitions( /D _VARIADIC_MAX=10 )
endif()
