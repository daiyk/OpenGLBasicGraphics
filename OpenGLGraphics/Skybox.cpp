#include "Skybox.h"
#include "stb_image.h"

Skybox::Skybox()
{
	mesh = std::make_unique<MeshData>();
	shader = std::make_unique<Shader>();
}

Skybox::Skybox(std::vector<std::string> faceLocations)
{
	mesh = std::make_unique<MeshData>();
	shader = std::make_unique<Shader>();

	// Load the skybox shader
	shader->CreateFromFiles("Shaders/Skybox_Vertex.vert", "Shaders/Skybox_Fragment.frag");
	shader->AssignUniformProjectLoc("projection");
	shader->AssignUniformViewLoc("view");
	uniformSkybox = glGetUniformLocation(shader->GetShaderLocation(), "skybox");
	// Set the projection and view matrices
	uniformProjection = shader->GetProjectionLocation();
	uniformView = shader->GetViewLocation();

	//texture
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nChannels;
	for (size_t i = 0; i < 6; i++) {
		unsigned char* texData = stbi_load(faceLocations[i].c_str(),&width,&height,&nChannels,0);
		if(!texData) {
			printf("Failed to find: %s\n", faceLocations[i].c_str());
			return;
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
		stbi_image_free(texData);
	}
	//set the texture parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// mesh setup
	unsigned int skyboxIndices[] = {
		//front
		0, 1, 2,
		2, 1, 3,
		//right
		2, 3, 5,
		5, 3, 7,
		//back
		5, 7, 4,
		4, 7, 6,
		//left
		4, 6, 0,
		0, 6, 1,
		//top
		4, 0, 5,
		5, 0, 2,
		//bottom
		1, 6, 3,
		3, 6, 7
	};

	float skyboxVertices[] = {
		-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f,
		-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, -1.0f, 0.0f,0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -1.0f,0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 1.0f,0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f,0.0f, 0.0f, 0.0f, 0.0f, 0.0f
	};

	mesh->CreateMesh(skyboxVertices, 64, skyboxIndices, 32);

}
void Skybox::DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	// the skybox should not move with the camera, so we remove the translation part of the view matrix
	//view matrix  = lookAt(cameraPos, cameraPos + cameraFront, cameraUp), so remove camera pos influence
	viewMatrix = glm::mat4(glm::mat3(viewMatrix)); // remove the translation part of the view matrix
	glDepthMask(GL_FALSE);
	shader->UseShader();

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glUniform1d(uniformSkybox, 0);
	shader->Validate();
	mesh->RenderMesh();
	glDepthMask(GL_TRUE);
}