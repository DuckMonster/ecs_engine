#pragma once
#include "../RenderData.h"

namespace Rendering
{
	class IRenderPipeline
	{
		//--------------------------------------------------- Static
	public:
		static IRenderPipeline* Get();

		template<class TPipeline>
		static IRenderPipeline* Create();

	private:
		static IRenderPipeline* Current;

		//--------------------------------------------------- Interface
	public:
		virtual void Render(const RenderManifest& manifest) = 0;
	};

	template<class TPipeline>
	IRenderPipeline* IRenderPipeline::Create()
	{
		static_assert(std::is_base_of<IRenderPipeline, TPipeline>::value, "Trying to create render pipeline with invalid type");
		Current = new TPipeline();
		return Current;
	}
}