#include "CorePCH.h"
#include "RenderPipeline.h"

using namespace Rendering;

IRenderPipeline* IRenderPipeline::Current = nullptr;

IRenderPipeline* IRenderPipeline::Get()
{
	return Current;
}
