#include "CorePCH.h"
#include "Deferred.h"
#include "Core/Tools/GLUtils.h"
#include "Core/Resource/MaterialResource.h"
#include "Core/Resource/ResourceManager.h"
#include "../ShaderHelper.h"
#include "../TextureHelper.h"

using namespace glm;
using namespace Rendering;

/**	Constructor
*******************************************************************************/
DeferredPipeline::DeferredPipeline()
{
	Debug_Log( "Deferred pipeline created" );
}

/**	Destructor
*******************************************************************************/
DeferredPipeline::~DeferredPipeline()
{
	Debug_Log( "Deferred pipeline destroyed" );
}

/**	Render
*******************************************************************************/
void DeferredPipeline::Render( const RenderManifest& manifest )
{
	Init();

	// Render GBuffer
	Render_GBuffer( manifest );
	Render_Shadows( manifest );
	Render_Lighting( manifest );

	{
		GLUtils::RenderTexture( m_LightBuffer.GetColorTexture( 0 ) );
	}

	glBindVertexArray( 0 );
}

/**	Render GBuffer
*******************************************************************************/
void Rendering::DeferredPipeline::Render_GBuffer( const RenderManifest& manifest )
{
	FrameBufferScope scope( m_GFrameBuffer );

	glClearColor( 0.f, 0.f, 0.f, 1.f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glCullFace(GL_BACK);

	for (uint32 i = 0; i < manifest.RenderList.size(); ++i)
	{
		const RenderableData& renderable = manifest.RenderList[i];
		GLuint shader = renderable.Material.ShaderHandle;

		ShaderHelper helper( shader );

		// Camera matrix is persistent through all meshes
		helper.Set( "u_Camera", manifest.CameraMatrix );

		for(uint32 i=0; i<renderable.Material.Textures.size(); ++i)
		{
			glBindTexture(GL_TEXTURE_2D, renderable.Material.Textures[i]);
		}

		for(uint32 i=0; i<renderable.Meshes.size(); ++i)
		{
			const MeshData& mesh = renderable.Meshes[i];

			glBindVertexArray( mesh.VertexObject );

			// Setup matrices
			helper.Set( "u_Model", renderable.ModelMatrix * mesh.Transform );
			helper.Set( "u_ModelNormal", renderable.NormalMatrix * mesh.Transform );

			// Draw!
			if ( mesh.UseElements )
				glDrawElements( mesh.DrawMode, mesh.DrawCount, GL_UNSIGNED_INT, nullptr );
			else
				glDrawArrays( mesh.DrawMode, 0, mesh.DrawCount );
		}
	}
}

/**	Render Shadows
*******************************************************************************/
void Rendering::DeferredPipeline::Render_Shadows( const RenderManifest& manifest )
{
	FrameBufferScope scope( m_ShadowBuffer );

	glClearColor( 0.f, 0.f, 0.f, 1.f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	ShaderHelper helper( m_ShadowMaterial->GetData().ShaderHandle );

	// TEMP LIGHT
	glm::mat4 tempLight = glm::ortho( -20.f, 20.f, -20.f, 20.f, -50.f, 50.f );
	tempLight *= glm::lookAt( glm::vec3( 1.f ), glm::vec3( 0.f ), glm::vec3( 0.f, 1.f, 0.f ) );
	helper.Set( "u_Camera", tempLight );
	//-----------

	for ( const RenderableData& renderable : manifest.RenderList )
	{
		for(uint32 i=0; i<renderable.Meshes.size(); ++i)
		{
			const MeshData& mesh = renderable.Meshes[i];

			glBindVertexArray( mesh.VertexObject );

			// Setup matrices
			helper.Set( "u_Model", renderable.ModelMatrix * mesh.Transform );
			helper.Set( "u_ModelNormal", renderable.NormalMatrix * mesh.Transform );

			// Draw!
			if ( mesh.UseElements )
				glDrawElements( mesh.DrawMode, mesh.DrawCount, GL_UNSIGNED_INT, nullptr );
			else
				glDrawArrays( mesh.DrawMode, 0, mesh.DrawCount );
		}
	}
}

/**	Render Lighting
*******************************************************************************/
void Rendering::DeferredPipeline::Render_Lighting( const RenderManifest& manifest )
{
	FrameBufferScope scope( m_LightBuffer );

	GLuint shaderHandle = m_LightingMaterial->GetData().ShaderHandle;

	glm::mat4 tempLight = glm::ortho( -20.f, 20.f, -20.f, 20.f, -50.f, 50.f );
	tempLight *= glm::lookAt( glm::vec3( 1.f ), glm::vec3( 0.f ), glm::vec3( 0.f, 1.f, 0.f ) );
	glm::vec3 tempLightDir = glm::normalize(-glm::vec3(1.f));

	ShaderHelper shaderHelper( shaderHandle );
	shaderHelper.Set( "u_Deferred.color", 0 );
	shaderHelper.Set( "u_Deferred.normal", 1 );
	shaderHelper.Set( "u_Deferred.depth", 2 );
	shaderHelper.Set( "u_Light.shadowBuffer", 3 );
	shaderHelper.Set( "u_Light.matrix", tempLight );
	shaderHelper.Set( "u_Light.direction",  tempLightDir);

	// View Info
	shaderHelper.Set( "u_ViewInfo.viewProjInv", manifest.CameraMatrixInv );

	// Bind all textures
	TextureHelper textureHelper;
	textureHelper.Bind( m_GFrameBuffer.GetColorTexture( 0 ), 0 );
	textureHelper.Bind( m_GFrameBuffer.GetColorTexture( 1 ), 1 );
	textureHelper.Bind( m_GFrameBuffer.GetDepthTexture(), 2 );
	textureHelper.Bind( m_ShadowBuffer.GetDepthTexture(), 3 );

	glDrawArrays( GL_QUADS, 0, 4 );
}

/**	Init
*******************************************************************************/
void Rendering::DeferredPipeline::Init()
{
	if ( IsValid() )
		return;

	// G Buffer
	if ( !m_GFrameBuffer.IsComplete() )
	{
		//m_GFrameBuffer.CreateDownScaled( 3 );
		m_GFrameBuffer.CreateDownScaled( 1 );
		m_GFrameBuffer.AddColorTexture( 3, true );
		m_GFrameBuffer.AddColorTexture( 3, true );
		m_GFrameBuffer.AddDepthTexture();
	}

	// Shadow Buffer
	if ( !m_ShadowBuffer.IsComplete() )
	{
		m_ShadowBuffer.Create( 2048, 2048 );
		m_ShadowBuffer.AddDepthTexture();
	}

	// Light Buffer
	if ( !m_LightBuffer.IsComplete() )
	{
		m_LightBuffer.CreateDownScaled( 3 );
		m_LightBuffer.AddColorTexture( 3, false );
	}

	// Light material
	if ( m_LightingMaterial == nullptr )
	{
		m_LightingMaterial = ResourceManager::GetInstance()->Load<MaterialResource>( "Resource/Material/Deferred/Light.json" );
	}

	// Shadow material
	if ( m_ShadowMaterial == nullptr )
	{
		m_ShadowMaterial = ResourceManager::GetInstance()->Load<MaterialResource>( "Resource/Material/Deferred/Shadow.json" );
	}

	EnsureValid();
}

/**	Is Valid
*******************************************************************************/
bool Rendering::DeferredPipeline::IsValid() const
{
	return m_GFrameBuffer.IsComplete() &&
		m_ShadowBuffer.IsComplete() &&
		m_LightingMaterial->IsValid() &&
		m_ShadowMaterial->IsValid();
}

bool Rendering::DeferredPipeline::EnsureValid() const
{
	bool valid = true;
	valid &= Ensure( m_GFrameBuffer.IsComplete() );
	valid &= Ensure( m_ShadowBuffer.IsComplete() );
	valid &= Ensure( m_LightingMaterial->IsValid() );
	valid &= Ensure( m_ShadowMaterial->IsValid() );

	return valid;
}

