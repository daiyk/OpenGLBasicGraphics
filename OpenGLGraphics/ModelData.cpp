#include "ModelData.h"
#include <string>
#include <glm/glm.hpp>
ModelData::ModelData() {

}
 
bool ModelData::LoadMesh(aiMesh* mesh, const aiScene* scene) {
	MeshData meshData = MeshData();
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	//load vertices
	for (size_t i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		// it is certain the mTexture has the same number of elements as mVertices, but it can be null if there are no textures
		if (mesh->mTextureCoords[0]) {
			vertex.texcoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		//normal exist for sure
		vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		vertices.push_back(vertex);
	}
	//load indices
	for (size_t i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	meshData.SetVertices(vertices);
	meshData.SetIndices(indices);
	_meshes.push_back(meshData);
	return true;
}


bool ModelData::LoadNode(aiNode* node, const aiScene* scene) {
	//load all meshes in the node
	for (size_t i = 0; i < node->mNumMeshes; i++) {
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}
	//load all children nodes. if they exist
	for (size_t i = 0; i < node->mNumChildren; i++) {
		LoadNode(node->mChildren[i], scene);
	}
	return true;
}

//NOTICE: this class depends on aiProcess_Triangulate | aiProcess_GenSmoothNormals to keep triangle mesh and normals for the mesh
bool ModelData::LoadModel(const std::string& fileName) {
	Assimp::Importer importer = Assimp::Importer();
	_scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
	if (!_scene) {
		printf("Model (%s) failed to load: %s", fileName.c_str(), importer.GetErrorString());
		return false;
	}

	//load successful, continue loading the nodes
	LoadNode(_scene->mRootNode, _scene);
	LoadMaterials(_scene);
}