#pragma once
#include "RenderPipeline.h"
#include "Core/Rendering/FrameBuffer.h"

class MaterialResource;

namespace Rendering
{
	class DeferredPipeline : public IRenderPipeline
	{
	public:
		DeferredPipeline();
		~DeferredPipeline();

		void Render( const RenderManifest& manifest ) override;

	private:
		void Render_GBuffer(const RenderManifest& manifest);
		void Render_Shadows(const RenderManifest& manifest);
		void Render_Lighting(const RenderManifest& manifest);

		void Init();
		bool IsValid() const;
		bool EnsureValid() const;

		FrameBuffer m_GFrameBuffer;
		FrameBuffer m_ShadowBuffer;
		FrameBuffer m_LightBuffer;

		MaterialResource* m_LightingMaterial = nullptr;
		MaterialResource* m_ShadowMaterial = nullptr;
	};
}