#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_UV;

uniform mat4 u_Camera;
uniform mat4 u_Model;
uniform mat4 u_ModelNormal;

out vec3 f_World;
out vec3 f_Normal;

void main()
{
	vec4 world = u_Model * vec4(a_Position, 1.0);
	gl_Position = u_Camera * world;

	f_World = world.xyz;
	f_Normal = (u_ModelNormal * vec4(a_Normal, 1.0)).xyz;
}