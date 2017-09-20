#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/assign/list_of.hpp>

#include "FindCppInclude.hpp"
#include "FindTtcnInclude.hpp"

namespace IncludeBrowser
{
using namespace ::testing;
using boost::assign::list_of;

template <typename Parser>
std::list<std::string> findIncludes(const std::string& p_line)
{   
    Parser parser;
    parser(p_line);
    return parser.m_includedFiles; 
}

std::list<std::string> findCppIncludes(const std::string& p_line)
{
    return findIncludes<FindCppInclude>(p_line);
}

std::list<std::string> findTtcnIncludes(const std::string& p_line)
{
    return findIncludes<FindTtcnInclude>(p_line);
}

TEST(FindIncludeTS, shouldFindCppInclude)
{
    ASSERT_THAT(findCppIncludes("#include <iostream>"),       ElementsAre("iostream"));
    ASSERT_THAT(findCppIncludes("#include \"iostream\""),     ElementsAre("iostream"));
    ASSERT_THAT(findCppIncludes("#include <iostream.hpp>"),   ElementsAre("iostream.hpp"));
    ASSERT_THAT(findCppIncludes("#include \"iostream.hpp\""), ElementsAre("iostream.hpp"));
}

TEST(FindIncludeTS, shouldFindCppIncludeWintSpecialChar)
{
    ASSERT_THAT(findCppIncludes("#include <__iostream_>"),    ElementsAre("__iostream_"));
    ASSERT_THAT(findCppIncludes("#include \"__iostream_\""),  ElementsAre("__iostream_"));
    ASSERT_THAT(findCppIncludes("#include <__iostream_>"),    ElementsAre("__iostream_"));
    ASSERT_THAT(findCppIncludes("#include \"__iost_ream.h\""), ElementsAre("__iost_ream.h"));
}

TEST(FindIncludeTS, shouldFindCppIncludeInLineWithSpaces)
{
    ASSERT_THAT(findCppIncludes(" #include <iostream>"),          ElementsAre("iostream"));
    ASSERT_THAT(findCppIncludes(" #include <iostream> "),         ElementsAre("iostream"));
    ASSERT_THAT(findCppIncludes("   #include  <iostream>    "),   ElementsAre("iostream"));
    ASSERT_THAT(findCppIncludes(" #include \"iostream\" "),       ElementsAre("iostream"));
    ASSERT_THAT(findCppIncludes("   #include  \"iostream\"    "), ElementsAre("iostream"));
}

TEST(FindIncludeTS, shouldNotFindCppInclude)
{
    ASSERT_TRUE(findCppIncludes("").empty());
    
    ASSERT_TRUE(findCppIncludes("#include<iostream>").empty());
    ASSERT_TRUE(findCppIncludes("#includ\"iostream\"").empty());

    ASSERT_TRUE(findCppIncludes("#include <iostream").empty());
    ASSERT_TRUE(findCppIncludes("#include iostream>").empty());
    
    ASSERT_TRUE(findCppIncludes("#include \"iostream").empty());
    ASSERT_TRUE(findCppIncludes("#include iostream\"").empty());
    
    ASSERT_TRUE(findCppIncludes("#includeiostream").empty());
    ASSERT_TRUE(findCppIncludes("#include iostream").empty());

    ASSERT_TRUE(findCppIncludes("include <iostream>").empty());
    ASSERT_TRUE(findCppIncludes("#inclide iostream").empty());

    ASSERT_TRUE(findCppIncludes("//#inclide iostream").empty());
    ASSERT_TRUE(findCppIncludes(" // #inclide iostream").empty());
    ASSERT_TRUE(findCppIncludes("/*#inclide iostream").empty());
    ASSERT_TRUE(findCppIncludes(" /* #inclide iostream").empty());
}

TEST(FindIncludeTS, shouldFindCppIncludeInLineWithInlineComment)
{
    ASSERT_THAT(findCppIncludes("#include <iostream> //ad"),  ElementsAre("iostream"));
    ASSERT_THAT(findCppIncludes("#include <iostream> /* ad"), ElementsAre("iostream"));
    ASSERT_THAT(findCppIncludes("#include <iostream>//ad"),   ElementsAre("iostream"));
    ASSERT_THAT(findCppIncludes("#include <iostream>/* ad"),  ElementsAre("iostream"));
}

TEST(FindIncludeTS, shouldFindTtcnInclude)
{
    ASSERT_THAT(findTtcnIncludes("import from moduleName all;"), ElementsAre("moduleName.ttcn3"));
    ASSERT_THAT(findTtcnIncludes("import from moduleName all"),  ElementsAre("moduleName.ttcn3"));
    ASSERT_THAT(findTtcnIncludes("import from moduleName { function f_executeAaShellCommand };"),  ElementsAre("moduleName.ttcn3"));
    ASSERT_THAT(findTtcnIncludes("import from moduleName { function f_executeAaShellCommand }"),  ElementsAre("moduleName.ttcn3"));
}

TEST(FindIncludeTS, shouldFindTtcnIncludeInLineWithSpaces)
{
    ASSERT_THAT(findTtcnIncludes("  import  from    moduleName   all  ;  "), ElementsAre("moduleName.ttcn3"));
    ASSERT_THAT(findTtcnIncludes(" import  from    moduleName  {  function  f_executeAaShellCommand   } ;  "),  ElementsAre("moduleName.ttcn3"));
}

TEST(FindIncludeTS, shouldNotFindTtcnInclude)
{
    ASSERT_TRUE(findTtcnIncludes("").empty());
    ASSERT_TRUE(findTtcnIncludes("from module all;").empty());
    ASSERT_TRUE(findTtcnIncludes("import  module all;").empty());
    ASSERT_TRUE(findTtcnIncludes("import from all;").empty());
    ASSERT_TRUE(findTtcnIncludes("import from module;").empty());
    ASSERT_TRUE(findTtcnIncludes("import frommodule all;").empty());
    ASSERT_TRUE(findTtcnIncludes("impot from module all;").empty());
}

TEST(FindIncludeTS, shouldFindTtcnIncludeInLineWithInlineComment)
{
    ASSERT_THAT(findTtcnIncludes("import from moduleName all; //ad"), ElementsAre("moduleName.ttcn3"));
    ASSERT_THAT(findTtcnIncludes("import from moduleName all;//ad"), ElementsAre("moduleName.ttcn3"));
    ASSERT_THAT(findTtcnIncludes("import from moduleName all; /* ad"), ElementsAre("moduleName.ttcn3"));
    ASSERT_THAT(findTtcnIncludes("import from moduleName all;/* ad"), ElementsAre("moduleName.ttcn3"));
}

}
