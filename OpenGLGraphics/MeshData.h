#pragma once
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

struct Vertex{
	glm::vec3 position;
	glm::vec2 texcoord;
	glm::vec3 normal;
};
class MeshData
{

	public:
		MeshData();
		~MeshData();
		void CreateMesh(GLfloat* verticesPtr, unsigned int vertics, GLuint* indicesPtr, unsigned int indices);
		void RenderMesh();
		void ClearMesh();
		bool SetVertices(std::vector<Vertex> vertices);
		bool SetIndices(std::vector<unsigned int> indices);
	private:
		GLuint VAO, VBO, EBO, shader, MoveLocation;
		GLsizei indexCount;
};

