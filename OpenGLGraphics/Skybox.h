#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "MeshData.h"
#include "Shader.h"

class Skybox
{
public:
	Skybox();
	Skybox(std::vector<std::string> faceLocations); // the face locations will be stored in a folder, so we need the name path of the face locations
	void DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
private:
	std::unique_ptr<MeshData> mesh;
	std::unique_ptr<Shader> shader;

	GLuint textureID;
	GLuint uniformProjection, uniformView;
	GLuint uniformSkybox;
};

