#pragma once
#include <gl/glew.h>
class MeshData
{

	public:
		MeshData();
		~MeshData();
		void CreateMesh(GLfloat* verticesPtr, unsigned int vertics, GLuint* indicesPtr, unsigned int indices);
		void RenderMesh();
		void ClearMesh();
	private:
		GLuint VAO, VBO, EBO, shader, MoveLocation;
		GLsizei indexCount;
};

