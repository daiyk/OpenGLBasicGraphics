#version 330
// the purpose of vertex shader is to transform the vertex position(gl_Position) from model space to clip space
// but we also need to pass the world space position of the vertex to the fragment shader
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 vCoord;
layout (location = 2) in vec3 vNormal;
out vec4 vCol;
out vec2 vTex;
out vec3 Normal;
out vec3 fragPos;
out vec4 lightSpaceFragPos; //for shadow mapping
uniform mat4 model; 
uniform mat4 projectMat; 
uniform mat4 viewMat;

uniform mat4 lightSpaceTransform; //for shadow mapping
void main()                     
{                              
	gl_Position = projectMat * viewMat * model * vec4(pos, 1.0);
	lightSpaceFragPos = lightSpaceTransform * model * vec4(pos, 1.0);
    vCol = vec4(clamp(pos,0.0f,1.0f),1.0f);
	vTex = vCoord;
    Normal = mat3(transpose(inverse(model))) * vNormal;
	fragPos = vec3(model * vec4(pos, 1.0));
};