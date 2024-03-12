#version 330                    
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 vCoord;
layout (location = 2) in vec3 vNormal;
out vec4 vCol;
out vec2 vTex;
out vec3 Normal;
out vec3 fragPos;
uniform mat4 model; 
uniform mat4 projectMat; 
uniform mat4 viewMat;
void main()                     
{                              
	gl_Position = projectMat * viewMat * model * vec4(pos, 1.0);
    vCol = vec4(clamp(pos,0.0f,1.0f),1.0f);
	vTex = vCoord;
    Normal = mat3(transpose(inverse(model))) * vNormal;
	fragPos = vec3(model * vec4(pos, 1.0));
};