#pragma once

#define TEST_EXPR(expr) do {\
bool result = !!(expr);\
Debug_Log("%s%s", #expr, result ? "" : " (FAIL)");\
if (!result)\
	return false;\
} while(0)\

#define DEFINE_TEST(function) \
namespace {\
Test::TestDefinition TEST_##function(#function, &function);\
}

class Test
{
public:
	/** Struct used to define a test function, declare these in C++ files to add a test
	*******************************************************************************/
	struct TestDefinition
	{
		typedef bool (*TestFunction)();
		TestDefinition(const char* file, TestFunction callback) : file(file), callback(callback)
		{
			Test::GetDefinitions().push_back(*this);
		}

		std::string file;
		TestFunction callback;
	};

public:
	static void Run();

protected:
	static std::vector<TestDefinition>& GetDefinitions();
};