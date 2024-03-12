#include "MeshData.h"
#include <stdio.h>

MeshData::MeshData()
{
	VAO = 0;
	VBO = 0;
	EBO = 0;
	shader = 0;
	MoveLocation = 0;
	indexCount = 0;
}

void MeshData::CreateMesh(GLfloat* verticesPtr, unsigned int vertics, GLuint* indicesPtr, unsigned int indices)
{
	indexCount = indices; // record the number of indices in the mesh
	glGenVertexArrays(1, &VAO); //generate the VAO
	printf("Current binding VAO is %i \n",VAO);
	glBindVertexArray(VAO); //bind the VAO
	glGenBuffers(1, &EBO); //generate the EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //bind the EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesPtr[0]) * indices, indicesPtr, GL_STATIC_DRAW); //pass the data to the EBO
	glGenBuffers(1, &VBO); //generate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //bind the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPtr[0]) * vertics, verticesPtr, GL_STATIC_DRAW); //pass the data to the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(verticesPtr[0]) * 8, 0); //set the vertex attribute pointers
	glEnableVertexAttribArray(0); //enable vertex attribute array
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(verticesPtr[0]) * 8, (void*)(sizeof(verticesPtr[0]) * 3)); //set the vertex attribute pointers
	glEnableVertexAttribArray(1); //enable vertex attribute array
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(verticesPtr[0]) * 8, (void*)(sizeof(verticesPtr[0]) * 5)); //set the vertex attribute pointers
	glEnableVertexAttribArray(2); //enable vertex attribute array
	glBindVertexArray(0); //unbind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind the VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //unbind the EBO
	
}
void MeshData::RenderMesh()
{
	if (indexCount > 0) {
		glBindVertexArray(VAO); //bind the VAO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //bind the EBO (this is not necessary since the VAO remembers the EBO	
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0); //draw the mesh
		glBindVertexArray(0); //unbind the VAO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //unbind the EBO	
	}
}
/// <summary>
/// clear the mesh so that it can be reloaded and rebind with other mesh data
/// </summary>
void MeshData::ClearMesh() {
	if (EBO != 0)
	{
		glDeleteBuffers(1, &EBO); //delete the EBO
		EBO = 0;
	}
	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO); //delete the VBO
		VBO = 0;
	}
	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO); //delete the VAO
		VAO = 0;
	}
	indexCount = 0;
}
//implement class in MeshData.h
MeshData::~MeshData()
{
	ClearMesh();
}
