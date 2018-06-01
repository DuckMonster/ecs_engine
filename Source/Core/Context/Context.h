#pragma once
#include "Core/Tools/SingletonClass.h"
#include "Core/Delegate/Delegate.h"

class Context : public SingletonClass<Context>
{
public:
	int x;
	int y;
	int width;
	int height;

	Delegate<void, int, int> OnWindowMoved;
	Delegate<void, int, int> OnResized;
};