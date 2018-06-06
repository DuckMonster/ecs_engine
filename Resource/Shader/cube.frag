#version 330 core
in vec3 f_World;
in vec3 f_Normal;
in vec2 f_UV;

uniform sampler2D u_Texture;

layout(location = 0) out vec3 o_Color;
layout(location = 1) out vec3 o_Normal;

void main()
{
	o_Color = texture(u_Texture, f_UV).xyz;
	//o_Color = vec3(f_UV, 0.0);
	o_Normal = f_Normal;
}