#pragma once
enum RenderFlag
{
	Deferred		= 1 << 0,
	Forward			= 1 << 1,
	CastShadow		= 1 << 2
};