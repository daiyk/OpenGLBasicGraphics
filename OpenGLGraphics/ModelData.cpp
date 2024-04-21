#include "ModelData.h"
#include "MeshData.h"
#include <string>
#include <glm/glm.hpp>
#include <filesystem>
ModelData::ModelData() {
	_scene = nullptr;
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
		else {
			vertex.texcoord = glm::vec2(0.0f, 0.0f);
		}
		//NOTE: normal exist for sure, because we are using aiProcess_GenSmoothNormals
		vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		vertices.push_back(vertex);
	}
	//load indices, NOTE: we are assuming the mesh is a triangle mesh
	for (size_t i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	_meshTomaterialsIndx.push_back(mesh->mMaterialIndex);
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
	_modelPath = std::filesystem::path(fileName);
	//load successful, continue loading the nodes
	LoadNode(_scene->mRootNode, _scene);
	LoadMaterials(_scene);
	return true;
}

bool ModelData::LoadMaterials(const aiScene* scene) {
	//load all materials if exists
	if(!scene->HasMaterials()) return false;
	for (size_t i = 0; i < scene->mNumMaterials; i++) {
		//load different types of materials

		aiMaterial* material = scene->mMaterials[i];
		Materials mat = Materials(material);
		//load diffuse textures
		LoadTextures(material, aiTextureType_DIFFUSE);
		//load specular textures
		LoadTextures(material, aiTextureType_SPECULAR);
		_materials.push_back(mat);
	}
	return true;
}

bool ModelData::LoadTextures(aiMaterial *material, aiTextureType type) {

	if(!material->GetTextureCount(type)) return false;
	for (size_t j = 0; j < material->GetTextureCount(type); j++) {
		aiString str;
		material->GetTexture(type, j, &str);
		//get the texture path, NOTE: texture must be local and in sub-directory of the model file
		Texture texture = Texture((_modelPath.remove_filename() / std::filesystem::path(str.C_Str()).filename()).string().c_str(), type);
		if (texture.LoadTexture()) {
			_textures.push_back(texture);
		}
		else {
			texture.ResetTexture("Textures/default.png");
			texture.LoadTexture();
			_textures.push_back(texture);
		}
	}
	return true;
}

void ModelData::RenderModel() {
	for (size_t i = 0; i < _meshes.size(); i++) {
		//get the material index for the mesh
		Texture texture = _textures[_meshTomaterialsIndx[i]];
		//render the mesh
		_meshes[i].RenderMesh();
	}
}