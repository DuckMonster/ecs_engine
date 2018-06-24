#include "CorePCH.h"
#include <GLFW/glfw3.h>
#include <chrono>
#include "Core/Context/Context.h"
#include "Core/Program/Program.h"
#include "Core/OS/File.h"
#include "Core/OS/Directory.h"

#ifdef TEST
#include "Core/Test/Test.h"
#endif

#ifdef TEST
// If running in test config, just run tests and quit
int main(int argv, char** argc)
{
	Test::Run();
	return 0;
}
#else
int main(int argv, char** argc)
{
	bool alwaysOnTop = false;

	for(int i=0; i<argv; ++i)
	{
		if (strcmp(argc[i], "top") == 0)
			alwaysOnTop = true;
	}

	// Set local root
	FDirectory::SetLocalDirectory(FDirectory::GetWorkingDirectory());

	//--------------------------------------------------- Create and init window

	Context* context = Context::Create();
	context->MakeCurrent();
	context->SetAlwaysOnTop(alwaysOnTop);

	Program program;
	context->OnFrame.BindLambda([&program]() { program.DoFrame(); });

	context->Run();
	delete context;

	return 0;
}	
#endif