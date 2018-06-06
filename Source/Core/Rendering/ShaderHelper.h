#pragma once
#include "RenderData.h"

#define SPEC_DECL(type)\
void Set(const char* uniform, const type& value)

class ShaderHelper
{
public:
	ShaderHelper( GLuint shader );
	~ShaderHelper();

	SPEC_DECL(GLint);
	SPEC_DECL(GLfloat);
	SPEC_DECL(glm::vec2);
	SPEC_DECL(glm::vec3);
	SPEC_DECL(glm::vec4);
	SPEC_DECL(glm::mat2);
	SPEC_DECL(glm::mat3);
	SPEC_DECL(glm::mat4);

	void SetMultiple(const std::vector<Rendering::UniformValue>& uniforms);
	void Set(const Rendering::UniformValue& uniform);

private:
	GLuint m_Shader;
};

#undef SPEC_DECL