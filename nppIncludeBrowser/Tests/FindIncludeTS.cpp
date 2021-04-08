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
	ASSERT_THAT(findCppIncludes(""), ElementsAre());
    
	ASSERT_THAT(findCppIncludes("#include<iostream>"), ElementsAre());
	ASSERT_THAT(findCppIncludes("#includ\"iostream\""), ElementsAre());

	ASSERT_THAT(findCppIncludes("#include <iostream"), ElementsAre());
	ASSERT_THAT(findCppIncludes("#include iostream>"), ElementsAre());
    
	ASSERT_THAT(findCppIncludes("#include \"iostream"), ElementsAre());
	ASSERT_THAT(findCppIncludes("#include iostream\""), ElementsAre());
    
	ASSERT_THAT(findCppIncludes("#includeiostream"), ElementsAre());
	ASSERT_THAT(findCppIncludes("#include iostream"), ElementsAre());

	ASSERT_THAT(findCppIncludes("include <iostream>"), ElementsAre());
	ASSERT_THAT(findCppIncludes("#inclide iostream"), ElementsAre());

	ASSERT_THAT(findCppIncludes("//#include <iostream>"), ElementsAre());
	ASSERT_THAT(findCppIncludes(" // #include <iostream>"), ElementsAre());
	ASSERT_THAT(findCppIncludes("/*#include <iostream>"), ElementsAre());
	ASSERT_THAT(findCppIncludes(" /* #include <iostream>"), ElementsAre());

	ASSERT_THAT(findCppIncludes("#include <>"), ElementsAre());
	ASSERT_THAT(findCppIncludes("#include \"\""), ElementsAre());
	ASSERT_THAT(findCppIncludes("#include </>"), ElementsAre());
	ASSERT_THAT(findCppIncludes("#include <\\>"), ElementsAre());
	ASSERT_THAT(findCppIncludes("#include <dir/>"), ElementsAre());
	ASSERT_THAT(findCppIncludes("#include <dir\\>"), ElementsAre());
}

TEST(FindIncludeTS, shouldFindCppIncludeInLineWithInlineComment)
{
    ASSERT_THAT(findCppIncludes("#include <iostream> //ad"),  ElementsAre("iostream"));
    ASSERT_THAT(findCppIncludes("#include <iostream> /* ad"), ElementsAre("iostream"));
    ASSERT_THAT(findCppIncludes("#include <iostream>//ad"),   ElementsAre("iostream"));
    ASSERT_THAT(findCppIncludes("#include <iostream>/* ad"),  ElementsAre("iostream"));

	ASSERT_THAT(findCppIncludes("#include <os/iostream> //ad"), ElementsAre("iostream"));
	ASSERT_THAT(findCppIncludes("#include <os/iostream> /* ad"), ElementsAre("iostream"));
	ASSERT_THAT(findCppIncludes("#include <os/iostream>//ad"), ElementsAre("iostream"));
	ASSERT_THAT(findCppIncludes("#include <os/iostream>/* ad"), ElementsAre("iostream"));
}

TEST(FindIncludeTs, shouldFindFileFromGivenPath)
{
	ASSERT_THAT(findCppIncludes("#include <dir/file.hpp>"), ElementsAre("file.hpp"));
	ASSERT_THAT(findCppIncludes("#include <dir/1dir2/file.hpp>"), ElementsAre("file.hpp"));
	ASSERT_THAT(findCppIncludes("#include </file1.hpp>"), ElementsAre("file1.hpp"));

	ASSERT_THAT(findCppIncludes("#include <dir\\file.hpp>"), ElementsAre("file.hpp"));
	ASSERT_THAT(findCppIncludes("#include <dir\\1dir2\\file.hpp>"), ElementsAre("file.hpp"));
	ASSERT_THAT(findCppIncludes("#include <\\file1.hpp>"), ElementsAre("file1.hpp"));
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
	ASSERT_THAT(findTtcnIncludes("  import  from\tmoduleName   all  ;  "), ElementsAre("moduleName.ttcn3"));
}

TEST(FindIncludeTS, shouldNotFindTtcnInclude)
{
	ASSERT_THAT(findTtcnIncludes(""), ElementsAre());
	ASSERT_THAT(findTtcnIncludes("from module all;"), ElementsAre());
	ASSERT_THAT(findTtcnIncludes("import  module all;"), ElementsAre());
	ASSERT_THAT(findTtcnIncludes("import from all;"), ElementsAre());
	ASSERT_THAT(findTtcnIncludes("import from module;"), ElementsAre());
	ASSERT_THAT(findTtcnIncludes("import frommodule all;"), ElementsAre());
	ASSERT_THAT(findTtcnIncludes("impot from module all;"), ElementsAre());
	ASSERT_THAT(findTtcnIncludes("//import from moduleName all;"), ElementsAre());
	ASSERT_THAT(findTtcnIncludes("// import from moduleName all;"), ElementsAre());
	ASSERT_THAT(findTtcnIncludes("/* import from moduleName all;"), ElementsAre());
}

TEST(FindIncludeTS, shouldFindTtcnIncludeInLineWithInlineComment)
{
    ASSERT_THAT(findTtcnIncludes("import from moduleName all; //ad"), ElementsAre("moduleName.ttcn3"));
    ASSERT_THAT(findTtcnIncludes("import from moduleName all;//ad"), ElementsAre("moduleName.ttcn3"));
    ASSERT_THAT(findTtcnIncludes("import from moduleName all; /* ad"), ElementsAre("moduleName.ttcn3"));
    ASSERT_THAT(findTtcnIncludes("import from moduleName all;/* ad"), ElementsAre("moduleName.ttcn3"));
}

}
