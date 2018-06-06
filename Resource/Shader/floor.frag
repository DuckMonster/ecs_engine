#version 330 core
in vec3 f_World;
in vec3 f_Normal;

layout(location = 0) out vec3 o_Color;
layout(location = 1) out vec3 o_Normal;

void main()
{
	o_Color = vec3(0.4, 0.25, 0.15);
	o_Normal = f_Normal;
}