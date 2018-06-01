#include "CorePCH.h"
#include "Test.h"
#include <iostream>

/**	Run all tests
*******************************************************************************/
void Test::Run()
{
	int totalTests = 0;
	int failedTests = 0;

	for(Test::TestDefinition& definition : GetDefinitions())
	{
		Debug_Log("### %s ###", definition.file.c_str());
		bool result = definition.callback();
		Debug_Log("### %s ###\n", result ? "SUCCESS" : "FAIL");

		totalTests++;
		if (!result)
			failedTests++;
	}

	Debug_Log("### TESTING COMPLETE ###");
	Debug_Log("%s (%d / %d)", failedTests == 0 ? "SUCCESS" : "FAIL", totalTests - failedTests, totalTests);
	std::cin.get();
}

/**	Get Definitions
*******************************************************************************/
std::vector<Test::TestDefinition>& Test::GetDefinitions()
{
	static std::vector<Test::TestDefinition> DefinitionList;
	return DefinitionList;
}

