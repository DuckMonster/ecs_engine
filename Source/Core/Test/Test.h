#pragma once

#define DEFINE_TEST(name, test) \
namespace {\
Test::TestDefinition name(#name, []() -> bool ## test ##);\
}\

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