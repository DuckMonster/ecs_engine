#pragma once

namespace Rendering
{
	enum class UniformType
	{
		Float,
		Int,
		Vec2,
		Vec3,
		Vec4
	};

	struct UniformValue
	{
		std::string Name;
		UniformType Type;

		union
		{
			float FloatData[16];
			int IntData[16];
		};
	};

	struct MaterialData
	{
		GLuint ShaderHandle;
		std::vector<UniformValue> UniformValues;
		std::vector<GLuint> Textures;
	};

	struct MeshData
	{
		GLuint VertexObject;
		bool UseElements;
		GLuint DrawCount;
		GLenum DrawMode;
	};

	struct RenderableData
	{
		MeshData Mesh;
		MaterialData Material;

		glm::mat4 ModelMatrix;
		glm::mat4 ModelMatrixInv;
		glm::mat4 NormalMatrix;
	};

	struct RenderManifest
	{
		glm::mat4 ProjectionMatrix;
		glm::mat4 ProjectionMatrixInv;
		glm::mat4 ViewMatrix;
		glm::mat4 ViewMatrixInv;
		glm::mat4 CameraMatrix;
		glm::mat4 CameraMatrixInv;
		std::vector<RenderableData> RenderList;
	};
}