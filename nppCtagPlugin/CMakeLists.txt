cmake_minimum_required(VERSION 2.8)

project(nppCTagPlugin)

add_definitions(-DUNICODE -D_UNICODE)

add_library(
    CTagsCore
    STATIC
    Source/CTagsController.cpp
    Source/CTagsNavigator.cpp
    Source/TagHierarchy.cpp
    Source/CTagsGenerator.cpp
    Source/MultipleTagFilesReader.cpp
    Source/FileScopedTagFilteringReader.cpp
    Source/ConfigGetter.cpp
    Source/Navigator.cpp
    Source/TagHolder.cpp
    Source/TagParser.cpp
    Source/ExtensionFields.cpp
    Source/TagPrinter.cpp
)

target_include_directories(
    CTagsCore
    PUBLIC
    ../Utils/Interfaces
    ../Utils/Include
    Include
    D:/boost_1_61_0
)

add_library(
    CTagsReader
    STATIC
    Source/ReadTagsProxy.cpp
    Source/TagFileReader.cpp
    Source/TagFile.cpp
    Source/CachedTagsReader.cpp
)

target_include_directories(
    CTagsReader
    PUBLIC
    ../Utils/Include
    Include
    D:/boost_1_61_0
)

add_library(
    CTagsTags
    STATIC
    Source/Tag.cpp
    Source/CppTag.cpp
    Source/CppIsTagWithAtt.cpp
    Source/GenericKindTag.cpp
    Source/TagBuilders.cpp
    Source/CppTagBuilder.cpp
    Source/GenericKindTagBuilder.cpp
)

target_include_directories(
    CTagsTags
    PUBLIC
    ../Utils/Include
    Include
    D:/boost_1_61_0
)

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

add_dependencies(
    nppCTagPlugin
    CTagsCore
    CTagsReader
    CTagsTags
    nppUtils
    winApiUtils
)

target_link_libraries(
    nppCTagPlugin
    CTagsCore
    CTagsReader
    CTagsTags
    nppUtils
    winApiUtils
    comctl32
)

add_executable(
    CTagsTests
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
    ../Utils/Include
    Include
    D:/boost_1_61_0
    D:/googletest-release-1.6.0/include
    D:/googlemock-release-1.6.0/include
)

add_dependencies(
    CTagsTests
    CTagsCore
    CTagsTags
)

target_link_libraries(
    CTagsTests
    CTagsCore
    CTagsTags
    gtest.lib
    gmock.lib
    gtest_main.lib
)

add_executable(
    CTagsMT
    ../Utils/Source/Log.cpp
    ../Utils/Source/Logger.cpp
    Tests/CTagsMT.cpp
    Tests/TestsMain.cpp
)

target_include_directories(
    CTagsMT
    PUBLIC
    ../Utils/Interfaces
    ../Utils/Include
    Include
    D:/boost_1_61_0
    D:/googletest-release-1.6.0/include
    D:/googlemock-release-1.6.0/include
)

add_dependencies(
    CTagsMT
    CTagsCore
    CTagsReader
    CTagsTags
)

target_link_libraries(
    CTagsMT
    CTagsCore
    CTagsReader
    CTagsTags
    D:/googletest-release-1.6.0/Build/Release/gtest.lib
    D:/googlemock-release-1.6.0/Build/Release/gmock.lib
)

add_executable(
    CTagsPT
    ../Utils/Source/Log.cpp
    ../Utils/Source/Logger.cpp
    Tests/CTagsPT.cpp
    Tests/TestsMain.cpp
)

target_include_directories(
    CTagsPT
    PUBLIC
    ../Utils/Interfaces
    ../Utils/Include
    Include
    D:/boost_1_61_0
    D:/googletest-release-1.6.0/include
    D:/googlemock-release-1.6.0/include
)

add_dependencies(
    CTagsPT
    CTagsCore
    CTagsReader
    CTagsTags
)

target_link_libraries(
    CTagsPT
    CTagsCore
    CTagsReader
    CTagsTags
    D:/googletest-release-1.6.0/Build/Release/gtest.lib
    D:/googlemock-release-1.6.0/Build/Release/gmock.lib
)

if( MSVC ) # VS2012 doesn't support correctly the tuples yet
    add_definitions( /D _VARIADIC_MAX=10 )
endif()
