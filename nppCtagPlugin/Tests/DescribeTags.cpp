#include "DescribeTags.hpp"

namespace CTagsPlugin
{
using namespace ::testing;

void describe(const Tag& p_expeced, MatchResultListener* p_listener)
{
	try
	{
		*p_listener << p_expeced.toString() << ". NOTE: not all fields are being matched\n";
	}
	catch (const std::exception& e)
	{
		*p_listener << "Unexpected exception: " << e.what() << "\n";
	}
}
void describe(const Tag& p_expeced, const Tag& p_actual, MatchResultListener* p_listener)
{
	try
	{
		*p_listener << "  Expected:\n";
		describe(p_expeced, p_listener);
		*p_listener << "  Actual:\n";
		describe(p_actual, p_listener);
	}
	catch (const std::exception& e)
	{
		*p_listener << "Unexpected exception: " << e.what() << "\n";
	}
}

}
