#include "CorePCH.h"
#include "MeshResource.h"

bool MeshResource::Load(const char* path)
{
	float data[] {
		-0.5f, -0.5f, 0.f,
		0.5f, -0.5f, 0.f,
		0.f, 0.5f, 0.f
	};

	GLuint vao, vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	{
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);

		glBindVertexArray(0);
	}

	m_VAO = vao;
	m_DrawMode = GL_TRIANGLES;
	m_DrawCount = 3;
	m_Elements = false;

	return Resource::Load(path);
}