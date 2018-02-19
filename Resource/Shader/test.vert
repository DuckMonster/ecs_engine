#version 330 core
layout(location = 0) in vec3 a_Position;
uniform mat4 u_Camera;
uniform mat4 u_Model;

out vec3 f_Position;

void main()
{
	vec4 world = u_Model * vec4(a_Position, 1.0);
	gl_Position = u_Camera * world;
	f_Position = world.xyz;
}