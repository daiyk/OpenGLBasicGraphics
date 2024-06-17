#version 330

layout(location = 0) in vec3 pos;

uniform mat4 model; //model matrix, from local space to world space


void main()
{
//we will have six pointlightspace transfrom in six directions so we dont need to pass lightSpaceTransform
	gl_Position = model * vec4(pos,1.0);
}