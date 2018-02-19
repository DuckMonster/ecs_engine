#pragma once
#include "Core/Utils/SingletonClass.h"

class Context : public SingletonClass<Context>
{
public:
	int x;
	int y;
	int width;
	int height;
};