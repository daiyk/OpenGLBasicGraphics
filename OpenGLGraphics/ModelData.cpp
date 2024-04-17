#include "ModelData.h"
#include <string>
ModelData::ModelData() {

}
 
//bool ModelData::LoadMesh(aiMesh* mesh, const aiScene* scene) {
//	MeshData meshData = MeshData();
//	std::vector<Vertex> vertices;
//	std::vector<unsigned int> indices;
//	//load vertices
//	for (size_t i = 0; i < mesh->mNumVertices; i++) {
//		Vertex vertex = Vertex();
//		vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
//		if (mesh->mTextureCoords[0]) {
//			vertex.texcoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
//		}
//		else {
//			vertex.texcoord = glm::vec2(0.0f, 0.0f);
//		}
//		vertices.push_back(vertex);
//	}
//	//load indices
//	for (size_t i = 0; i < mesh->mNumFaces; i++) {
//		aiFace face = mesh->mFaces[i];
//		for (size_t j = 0; j < face.mNumIndices; j++) {
//			indices.push_back(face.mIndices[j]);
//		}
//	}
//	meshData.SetVertices(vertices);
//	meshData.SetIndices(indices);
//	_meshes.push_back(meshData);
//	return true;
//}


bool ModelData::LoadNode(aiNode* node, const aiScene* scene) {
	//load all meshes in the node
	for (size_t i = 0; i < node->mNumMeshes; i++) {
		//LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}
	//load all children nodes
	for (size_t i = 0; i < node->mNumChildren; i++) {
		LoadNode(node->mChildren[i], scene);
	}
	return true;
}
bool ModelData::LoadModel(const std::string& fileName) {
	Assimp::Importer importer = Assimp::Importer();
	_scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
	if (!_scene) {
		printf("Model (%s) failed to load: %s", fileName.c_str(), importer.GetErrorString());
		return false;
	}

	//load successful, continue loading the nodes
	LoadNode(_scene->mRootNode, _scene);
}