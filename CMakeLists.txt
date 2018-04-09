cmake_minimum_required(VERSION 2.8)

project(NppPlugins)

add_definitions(-DUNICODE -D_UNICODE)

link_directories(
    D:/boost_1_61_0/stage/lib
    D:/googletest-release-1.6.0/Build/Debug
    D:/googlemock-release-1.6.0/Build/Debug
)

add_subdirectory(WinApi)
add_subdirectory(Utils)
add_subdirectory(nppOpenFile)
add_subdirectory(nppIncludeBrowser)
add_subdirectory(nppHeaderSourceSwitcher)
add_subdirectory(nppCTagPlugin)
add_subdirectory(nppProjectPlugin)


if( MSVC ) # VS2012 doesn't support correctly the tuples yet
    add_definitions( /D _VARIADIC_MAX=10 )
endif()
