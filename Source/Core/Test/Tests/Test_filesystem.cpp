#include <CorePCH.h>
#include "../Test.h"
#include "Core/Tools/Directory.h"

#define PRINT_DIR_OPERATION(dir, expr) do { std::string prev = dir.ToString(); expr; Debug_Log("(%s) => (%s) => (%s)", prev.c_str(), #expr, dir.ToString().c_str()); } while(0)

bool Directory()
{
	FDirectory directory;

	TEST_EXPR(directory.IsRoot() == false);
	PRINT_DIR_OPERATION(directory, directory = "/Hello/This\\Is\\Dog//");
	PRINT_DIR_OPERATION(directory, directory += "/More/Stuff\\");

	return true;
}

DEFINE_TEST(Directory);