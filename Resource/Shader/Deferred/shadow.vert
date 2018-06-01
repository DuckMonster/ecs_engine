#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;

uniform mat4 u_Camera;
uniform mat4 u_Model;
uniform mat4 u_ModelNormal;

void main()
{
	vec4 world = u_Model * vec4(a_Position, 1.0);
	gl_Position = u_Camera * world;
}