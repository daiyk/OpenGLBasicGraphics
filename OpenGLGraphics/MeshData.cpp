#include "MeshData.h"
#include <stdio.h>

MeshData::MeshData()
{
	glGenVertexArrays(1, &VAO); //generate the VAO
	printf("A new VAO is generated: %i \n", VAO);
	VBO = 0;
	EBO = 0;
	shader = 0;
	MoveLocation = 0;
	indexCount = 0;
}

void MeshData::CreateMesh(GLfloat* verticesPtr, unsigned int verticsElementNum, GLuint* indicesPtr, unsigned int indices)
{
	indexCount = indices; // record the number of indices in the mesh
	printf("Current binding VAO is %i \n",VAO);
	glBindVertexArray(VAO); //bind the VAO
	glGenBuffers(1, &EBO); //generate the EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //bind the EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesPtr[0]) * indices, indicesPtr, GL_STATIC_DRAW); //pass the data to the EBO
	glGenBuffers(1, &VBO); //generate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //bind the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPtr[0]) * verticsElementNum, verticesPtr, GL_STATIC_DRAW); //pass the data to the VBO
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

bool MeshData::SetVertices(std::vector<Vertex> vertices) {
	//vertices must have at least one vertex, otherwise return false
	if (vertices.size() == 0) return false;
	indexCount = vertices.size();
	//allocate memory for the vertices, 3 positions, 2 texcoords, 3 normals
	GLfloat* verticesPtr = new GLfloat[vertices.size() * 8];
	//copy the vertices to the verticesPtr
	for (int i = 0; i < vertices.size(); i++)
	{
		verticesPtr[i * 8] = vertices[i].position.x;
		verticesPtr[i * 8 + 1] = vertices[i].position.y;
		verticesPtr[i * 8 + 2] = vertices[i].position.z;
		verticesPtr[i * 8 + 3] = vertices[i].texcoord.x;
		verticesPtr[i * 8 + 4] = vertices[i].texcoord.y;
		verticesPtr[i * 8 + 5] = vertices[i].normal.x;
		verticesPtr[i * 8 + 6] = vertices[i].normal.y;
		verticesPtr[i * 8 + 7] = vertices[i].normal.z;
	}
	unsigned int verticsElementNum = vertices.size() * 8;
	//set up the vertex data
	glBindVertexArray(VAO); //bind the VAO
	glGenBuffers(1, &VBO); //generate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //bind the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPtr[0]) * verticsElementNum, verticesPtr, GL_STATIC_DRAW); //pass the data to the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(verticesPtr[0]) * 8, 0); //set the vertex attribute pointers for position
	glEnableVertexAttribArray(0); //enable vertex attribute array
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(verticesPtr[0]) * 8, (void*)(sizeof(verticesPtr[0]) * 3)); //set the vertex attribute pointers for textCoord, offset is 3
	glEnableVertexAttribArray(1); //enable vertex attribute array
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(verticesPtr[0]) * 8, (void*)(sizeof(verticesPtr[0]) * 5)); //set the vertex attribute pointers for normal, offset is 5
	glEnableVertexAttribArray(2); //enable vertex attribute array
	glBindVertexArray(0); //unbind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind the VBO
	//delete[] verticesPtr; //delete the verticesPtr
	return true;
}

bool MeshData::SetIndices(std::vector<unsigned int> indices) {
	//indices must have at least one index, otherwise return false
	if (indices.size() == 0) return false;
	//allocate memory for the indices
	GLuint* indicesPtr = new GLuint[indices.size()];
	//copy the indices to the indicesPtr
	for (int i = 0; i < indices.size(); i++)
	{
		indicesPtr[i] = indices[i];
	}
	unsigned int indicesNum = indices.size();
	//set up the indices data
	glBindVertexArray(VAO); //bind the VAO
	glGenBuffers(1, &EBO); //generate the EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //bind the EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesPtr[0]) * indicesNum, indicesPtr, GL_STATIC_DRAW); //pass the data to the EBO
	glBindVertexArray(0); //unbind the VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //unbind the EBO
	//delete[] indicesPtr; //delete the indicesPtr
	return true;
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
