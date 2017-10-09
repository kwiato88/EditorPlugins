cmake_minimum_required(VERSION 2.8)

project(nppCTagPlugin)

add_definitions(-DUNICODE -D_UNICODE)

add_library(
    nppCTagPlugin
    SHARED
    Source/dllmain.cpp
    Source/Plugin.cpp
    Source/CppSearchDialog.cpp
    Source/TagHierarchyDialog.cpp
    Source/ListViewTagsSelector.cpp
    Source/GridViewTagsSelector.cpp
    Source/TreeViewTagHierSelector.cpp
    # CORE
    Source/CTagsController.cpp
    Source/CTagsNavigator.cpp
    Source/TagHierarchy.cpp
    Source/CTagsGenerator.cpp
    Source/ReadTagsProxy.cpp
    Source/TagFileReader.cpp
    Source/TagFile.cpp
    Source/MultipleTagFilesReader.cpp
    Source/FileScopedTagFilteringReader.cpp
    Source/CachedTagsReader.cpp
    Source/ConfigGetter.cpp
    Source/Navigator.cpp
    Source/TagHolder.cpp
    Source/TagParser.cpp
    Source/ExtensionFields.cpp
    Source/TagPrinter.cpp
    # TAGS
    Source/Tag.cpp
    Source/CppTag.cpp
    Source/CppIsTagWithAtt.cpp
    Source/GenericKindTag.cpp
    Source/TagBuilders.cpp
    Source/CppTagBuilder.cpp
    Source/GenericKindTagBuilder.cpp
    # RESOURCES
    ../WinApi/Resource/ListBoxDialog.rc
    ../WinApi/Resource/GridDialog.rc
    ../WinApi/Resource/TreeDialog.rc
    ../WinApi/Resource/SelectPathsDialog.rc
    Resource/CppSearchDialog.rc
    Resource/TagHierarchyDialog.rc
    Resource/CTagsPluginDetail.rc
)

target_include_directories(
    nppCTagPlugin
    PUBLIC
    ../PluginInterface
    ../Utils/Interfaces
    ../Utils/Include
    ../WinApi/Include
    Include
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
    Source/Tag.cpp
    Source/CppTag.cpp
    Source/CppIsTagWithAtt.cpp
    Source/GenericKindTag.cpp
    Source/TagHolder.cpp
    Source/TagParser.cpp
    Source/TagBuilders.cpp
    Source/ExtensionFields.cpp
    Source/TagPrinter.cpp
    Source/CppTagBuilder.cpp
    Source/GenericKindTagBuilder.cpp
    Source/Navigator.cpp
    Source/TagHierarchy.cpp
    Tests/TagParserTS.cpp
    Tests/CppTagTS.cpp
    Tests/CppTagBuilderTS.cpp
    Tests/GenericKindTagBuilderTS.cpp
    Tests/NavigatorTS.cpp
    Tests/TagHierarchyTS.cpp
)

target_include_directories(
    CTagsTests
    PUBLIC
    ../PluginInterface
    ../Utils/Interfaces
    ../Utils/Include
    Include
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
    Source/CTagsNavigator.cpp
    Source/TagHierarchy.cpp
    Source/TagFileReader.cpp
    Source/TagFile.cpp
    Source/CachedTagsReader.cpp
    Source/Navigator.cpp
    ../Utils/Source/Log.cpp
    ../Utils/Source/Logger.cpp
    # TAGS
    Source/Tag.cpp
    Source/CppTag.cpp
    Source/CppIsTagWithAtt.cpp
    Source/GenericKindTag.cpp
    Source/TagHolder.cpp
    Source/TagParser.cpp
    Source/TagBuilders.cpp
    Source/ExtensionFields.cpp
    Source/CppTagBuilder.cpp
    Source/GenericKindTagBuilder.cpp
    Source/TagPrinter.cpp
    # TESTS
    Tests/CTagsMT.cpp
)

target_include_directories(
    CTagsMT
    PUBLIC
    ../PluginInterface
    ../Utils/Interfaces
    ../Utils/Include
    Include
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
    Source/CTagsNavigator.cpp
    Source/TagHierarchy.cpp
    Source/TagFileReader.cpp
    Source/TagFile.cpp
    Source/CachedTagsReader.cpp
    Source/Navigator.cpp
    ../Utils/Source/Log.cpp
    ../Utils/Source/Logger.cpp
    # TAGS
    Source/Tag.cpp
    Source/CppTag.cpp
    Source/GenericKindTag.cpp
    Source/TagHolder.cpp
    Source/TagParser.cpp
    Source/TagBuilders.cpp
    Source/ExtensionFields.cpp
    Source/CppTagBuilder.cpp
    Source/GenericKindTagBuilder.cpp
    Source/TagPrinter.cpp
    # TESTS
    Tests/CTagsPT.cpp
)

target_include_directories(
    CTagsPT
    PUBLIC
    ../PluginInterface
    ../Utils/Interfaces
    ../Utils/Include
    Include
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
