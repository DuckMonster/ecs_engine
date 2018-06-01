#version 330 core
struct DeferredBuffers
{
	sampler2D color;
	sampler2D normal;
	sampler2D depth;
};

struct Light
{
	sampler2D shadowBuffer;
	mat4 matrix;
	mat4 matrixInv;
};

struct ViewInfo
{
	mat4 viewProjInv;
};

uniform DeferredBuffers u_Deferred;
uniform Light u_Light;
uniform ViewInfo u_ViewInfo;

in vec2 f_UV;
out vec4 o_Color;

vec3 depthToWorld(float depth)
{
	// First step! Get the NDC, which would be the result of a world -> screen projection
	vec4 ndc = vec4(f_UV.x, f_UV.y, depth, 1.0);

	// [0, 1] -> [-1, 1]
	ndc = (ndc - 0.5) * 2.0;

	// Deproject that bad-boy
	ndc = u_ViewInfo.viewProjInv * ndc;

	return ndc.xyz;
}

float shadow(float depth, vec3 normal)
{
	vec3 world = depthToWorld(depth);

	// Bias with normal
	world += normal * 0.05;

	// Project world into light-space
	vec4 lightSpace = u_Light.matrix * vec4(world, 1.0);
	lightSpace /= lightSpace.w;

	// [-1, 1] -> [0, 1]
	lightSpace = (lightSpace + 1.0) / 2.0;
	float lightDepth = texture(u_Light.shadowBuffer, vec2(lightSpace.x, lightSpace.y)).x;

	// Bias the lightdepth a bit as well
	//lightDepth += 0.0016;

	return step(lightDepth, lightSpace.z);
	//return lightDepth;
}

void main()
{
	// Get deferred values
	vec3 color = texture(u_Deferred.color, f_UV).xyz;
	vec3 normal = texture(u_Deferred.normal, f_UV).xyz;
	float depth = texture(u_Deferred.depth, f_UV).x;

	float shadow = shadow(depth, normal);

	//o_Color = texture((u_Deferred.color), f_UV) / 5.0;
	//o_Color = vec4(shadow(), shadow(), shadow(), 1.0);
	//o_Color = vec4(shadow().x, 0.0, 0.0, 1.0);
	//o_Color = texture(u_Deferred.color, f_UV) * (1.0 - shadow());
	//o_Color = texture(u_Light.shadowBuffer, f_UV);

	o_Color = vec4(color * (1.0 - shadow), 1.0);

	//o_Color = vec4(depthToWorld(texture(u_Deferred.depth, f_UV).x), 1.0);
	//o_Color = texture(u_Deferred.depth, f_UV);
}