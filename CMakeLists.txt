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

add_library(
    nppCTagPlugin
    SHARED
    nppCtagPlugin/Source/dllmain.cpp
    nppCtagPlugin/Source/Plugin.cpp
    nppCtagPlugin/Source/CppSearchDialog.cpp
    nppCtagPlugin/Source/TagHierarchyDialog.cpp
    nppCtagPlugin/Source/ListViewTagsSelector.cpp
    nppCtagPlugin/Source/GridViewTagsSelector.cpp
    nppCtagPlugin/Source/TreeViewTagHierSelector.cpp
    # CORE
    nppCtagPlugin/Source/CTagsController.cpp
    nppCtagPlugin/Source/CTagsNavigator.cpp
    nppCtagPlugin/Source/TagHierarchy.cpp
    nppCtagPlugin/Source/CTagsGenerator.cpp
    nppCtagPlugin/Source/ReadTagsProxy.cpp
    nppCtagPlugin/Source/TagFileReader.cpp
    nppCtagPlugin/Source/TagFile.cpp
    nppCtagPlugin/Source/MultipleTagFilesReader.cpp
    nppCtagPlugin/Source/FileScopedTagFilteringReader.cpp
    nppCtagPlugin/Source/CachedTagsReader.cpp
    nppCtagPlugin/Source/ConfigGetter.cpp
    nppCtagPlugin/Source/Navigator.cpp
    nppCtagPlugin/Source/TagHolder.cpp
    nppCtagPlugin/Source/TagParser.cpp
    nppCtagPlugin/Source/ExtensionFields.cpp
    nppCtagPlugin/Source/TagPrinter.cpp
    # TAGS
    nppCtagPlugin/Source/Tag.cpp
    nppCtagPlugin/Source/CppTag.cpp
    nppCtagPlugin/Source/CppIsTagWithAtt.cpp
    nppCtagPlugin/Source/GenericKindTag.cpp
    nppCtagPlugin/Source/TagBuilders.cpp
    nppCtagPlugin/Source/CppTagBuilder.cpp
    nppCtagPlugin/Source/GenericKindTagBuilder.cpp
    # RESOURCES
    WinApi/Resource/ListBoxDialog.rc
    WinApi/Resource/GridDialog.rc
    WinApi/Resource/TreeDialog.rc
    WinApi/Resource/SelectPathsDialog.rc
    Resource/CppSearchDialog.rc
    Resource/TagHierarchyDialog.rc
    Resource/CTagsPluginDetail.rc
)

target_include_directories(
    nppCTagPlugin
    PUBLIC
    PluginInterface
    Utils/Interfaces
    Utils/Include
    WinApi/Include
    nppCtagPlugin/Include
    D:/boost_1_61_0
)

target_link_libraries(
    nppCTagPlugin
    nppUtils
    winApiUtils
    comctl32
)

add_executable(
    CTagsTests
    nppCtagPlugin/Source/Tag.cpp
    nppCtagPlugin/Source/CppTag.cpp
    nppCtagPlugin/Source/CppIsTagWithAtt.cpp
    nppCtagPlugin/Source/GenericKindTag.cpp
    nppCtagPlugin/Source/TagHolder.cpp
    nppCtagPlugin/Source/TagParser.cpp
    nppCtagPlugin/Source/TagBuilders.cpp
    nppCtagPlugin/Source/ExtensionFields.cpp
    nppCtagPlugin/Source/TagPrinter.cpp
    nppCtagPlugin/Source/CppTagBuilder.cpp
    nppCtagPlugin/Source/GenericKindTagBuilder.cpp
    nppCtagPlugin/Source/Navigator.cpp
    nppCtagPlugin/Source/TagHierarchy.cpp
    nppCtagPlugin/Tests/TagParserTS.cpp
    nppCtagPlugin/Tests/CppTagTS.cpp
    nppCtagPlugin/Tests/CppTagBuilderTS.cpp
    nppCtagPlugin/Tests/GenericKindTagBuilderTS.cpp
    nppCtagPlugin/Tests/NavigatorTS.cpp
    nppCtagPlugin/Tests/TagHierarchyTS.cpp
)

target_include_directories(
    CTagsTests
    PUBLIC
    PluginInterface
    Utils/Interfaces
    Utils/Include
    nppCtagPlugin/Include
    D:/boost_1_61_0
    D:/googletest-release-1.6.0/include
    D:/googlemock-release-1.6.0/include
)

target_link_libraries(
    CTagsTests
    gtest.lib
    gmock.lib
    gtest_main.lib
)

add_executable(
    CTagsMT
    nppCtagPlugin/Source/CTagsNavigator.cpp
    nppCtagPlugin/Source/TagHierarchy.cpp
    nppCtagPlugin/Source/TagFileReader.cpp
    nppCtagPlugin/Source/TagFile.cpp
    nppCtagPlugin/Source/CachedTagsReader.cpp
    nppCtagPlugin/Source/Navigator.cpp
    Utils/Source/Log.cpp
    Utils/Source/Logger.cpp
    # TAGS
    nppCtagPlugin/Source/Tag.cpp
    nppCtagPlugin/Source/CppTag.cpp
    nppCtagPlugin/Source/CppIsTagWithAtt.cpp
    nppCtagPlugin/Source/GenericKindTag.cpp
    nppCtagPlugin/Source/TagHolder.cpp
    nppCtagPlugin/Source/TagParser.cpp
    nppCtagPlugin/Source/TagBuilders.cpp
    nppCtagPlugin/Source/ExtensionFields.cpp
    nppCtagPlugin/Source/CppTagBuilder.cpp
    nppCtagPlugin/Source/GenericKindTagBuilder.cpp
    nppCtagPlugin/Source/TagPrinter.cpp
    # TESTS
    nppCtagPlugin/Tests/CTagsMT.cpp
)

target_include_directories(
    CTagsMT
    PUBLIC
    PluginInterface
    Utils/Interfaces
    Utils/Include
    nppCtagPlugin/Include
    D:/boost_1_61_0
    D:/googletest-release-1.6.0/include
    D:/googlemock-release-1.6.0/include
)

target_link_libraries(
    CTagsMT
    D:/googletest-release-1.6.0/Build/Release/gtest.lib
    D:/googlemock-release-1.6.0/Build/Release/gmock.lib
    D:/googletest-release-1.6.0/Build/Release/gtest_main.lib
)

add_executable(
    CTagsPT
    nppCtagPlugin/Source/CTagsNavigator.cpp
    nppCtagPlugin/Source/TagHierarchy.cpp
    nppCtagPlugin/Source/TagFileReader.cpp
    nppCtagPlugin/Source/TagFile.cpp
    nppCtagPlugin/Source/CachedTagsReader.cpp
    nppCtagPlugin/Source/Navigator.cpp
    Utils/Source/Log.cpp
    Utils/Source/Logger.cpp
    # TAGS
    nppCtagPlugin/Source/Tag.cpp
    nppCtagPlugin/Source/CppTag.cpp
    nppCtagPlugin/Source/GenericKindTag.cpp
    nppCtagPlugin/Source/TagHolder.cpp
    nppCtagPlugin/Source/TagParser.cpp
    nppCtagPlugin/Source/TagBuilders.cpp
    nppCtagPlugin/Source/ExtensionFields.cpp
    nppCtagPlugin/Source/CppTagBuilder.cpp
    nppCtagPlugin/Source/GenericKindTagBuilder.cpp
    nppCtagPlugin/Source/TagPrinter.cpp
    # TESTS
    nppCtagPlugin/Tests/CTagsPT.cpp
)

target_include_directories(
    CTagsPT
    PUBLIC
    PluginInterface
    Utils/Interfaces
    Utils/Include
    nppCtagPlugin/Include
    D:/boost_1_61_0
    D:/googletest-release-1.6.0/include
)

target_link_libraries(
    CTagsPT
    D:/googletest-release-1.6.0/Build/Release/gtest.lib
    D:/googletest-release-1.6.0/Build/Release/gtest_main.lib
)

if( MSVC ) # VS2012 doesn't support correctly the tuples yet
    add_definitions( /D _VARIADIC_MAX=10 )
endif()
