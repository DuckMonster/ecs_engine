#include "CorePCH.h"
#include "ShaderHelper.h"

#define SPEC_IMPL(type, ptrtype, func)\
void ShaderHelper::Set( const char* uniform, const type& value )\
{\
	GLint uniformHandle = glGetUniformLocation(m_Shader, uniform);\
	if (uniformHandle < 0)\
	{\
		/*Debug_Log("Uniform '%s' wasn't found", uniform); */\
		return;\
	}\
\
	func(uniformHandle, 1, (ptrtype*)&value);\
}

#define SPEC_IMPL_MAT(type, func)\
void ShaderHelper::Set( const char* uniform, const type& value )\
{\
	GLint uniformHandle = glGetUniformLocation(m_Shader, uniform);\
	if (uniformHandle < 0)\
	{\
		/*Debug_Log("Uniform '%s' wasn't found", uniform); */\
		return;\
	}\
\
	func(uniformHandle, 1, false, glm::value_ptr(value));\
}


ShaderHelper::ShaderHelper( GLuint shader )
	: m_Shader(shader)
{
	Ensure(shader != GLuint(-1));
	glUseProgram(shader);
}

ShaderHelper::~ShaderHelper()
{
}

SPEC_IMPL(GLint, GLint, glUniform1iv)
SPEC_IMPL(GLfloat, GLfloat, glUniform1fv)
SPEC_IMPL(glm::vec2, GLfloat, glUniform2fv)
SPEC_IMPL(glm::vec3, GLfloat, glUniform3fv)
SPEC_IMPL(glm::vec4, GLfloat, glUniform4fv)
SPEC_IMPL_MAT(glm::mat2, glUniformMatrix2fv)
SPEC_IMPL_MAT(glm::mat3, glUniformMatrix3fv)
SPEC_IMPL_MAT(glm::mat4, glUniformMatrix4fv)