#include "ModelData.h"
#include "MeshData.h"
#include <string>
#include <glm/glm.hpp>
#include <filesystem>
ModelData::ModelData() {

}
 
bool ModelData::LoadMesh(aiMesh* mesh, const aiScene* scene) {
	MeshData* meshData = new MeshData();
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
	meshData->SetVertices(vertices);
	meshData->SetIndices(indices);
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
bool ModelData::LoadModel(const std::string fileName) {
	Assimp::Importer importer = Assimp::Importer();
	//NOTE: scene will be destroyed when the this function ends
	const aiScene* _scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
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
		Materials* mat = new Materials(material);

		/// --- by default, we only load DIFFUSE and SPECULAR textures --- ///
		//load diffuse types of textures
		LoadTextures(material, aiTextureType_DIFFUSE, mat->textureIndices);
		//load specular textures
		LoadTextures(material, aiTextureType_SPECULAR, mat->textureIndices);
		_materials.push_back(mat);
	}
	return true;
}
/// <summary>
/// Not parallel safe, because it modifies the _textures vector
/// </summary>
/// <param name="material"></param>
/// <param name="type"></param>
/// <param name="indices"></param>
/// <returns></returns>
bool ModelData::LoadTextures(aiMaterial *material, aiTextureType type, std::vector<size_t> &indices) {

	if(!material->GetTextureCount(type)) return false;
	size_t textureIndice = _textures.size();
	for (size_t j = 0; j < material->GetTextureCount(type); j++) {
		aiString str;
		material->GetTexture(type, j, &str);
		//get the texture path, NOTE: texture must be local and in sub-directory of the model file
		std::string fileLocation = (_modelPath.parent_path() / std::filesystem::path(str.C_Str()).filename()).string();
		Texture *texture = new Texture(fileLocation.c_str(), type);
		if (texture->LoadTexture()) {
			_textures.push_back(texture);
			indices.push_back(textureIndice++);
		}
		else {
			texture->ResetTexture("Texture/default.jpg");
			texture->LoadTexture();
			_textures.push_back(texture);
			indices.push_back(textureIndice++);
		}
	}
	return true;
}

//render every mesh in this model
void ModelData::RenderModel(Shader &shader) {
	for (size_t i = 0; i < _meshes.size(); i++) {
		//get the material index for the mesh
		Materials material = *_materials[_meshTomaterialsIndx[i]];
		///------- bind the sampler with the texture unit ---- ///
		int diffuseNr = 0;
		int specularNr = 0;
		//only if it exists multiple texture types
		for (size_t j = 0; j < material.textureIndices.size(); j++) {
			_textures[material.textureIndices[j]]-> UseTexture(j);
			if (_textures[material.textureIndices[j]]->GetTextureType() == aiTextureType_DIFFUSE) {
				//bind the uniform sampler to the texture unit, so the shader can use it, with name "texture_diffuse"+diffuseNr
				glUniform1i(glGetUniformLocation(shader.GetShaderLocation(), ("texture_diffuse" + std::to_string(diffuseNr++)).c_str()), j);
			}
			else if (_textures[material.textureIndices[j]]->GetTextureType() == aiTextureType_SPECULAR) {
				glUniform1i(glGetUniformLocation(shader.GetShaderLocation(), ("texture_specular" + std::to_string(specularNr++)).c_str()), j);
			}
		}
		//render the mesh
		_meshes[i]->RenderMesh();
		//unbind the texture
		glActiveTexture(GL_TEXTURE0);
	}
}

void ModelData::ClearModel() {
	for (size_t i = 0; i < _meshes.size(); i++) {
		_meshes[i]->ClearMesh();
		delete _meshes[i];
	}
	for (size_t i = 0; i < _textures.size(); i++) {
		_textures[i]->ClearTexture();
		delete _textures[i];
	}
	for (size_t i = 0; i < _materials.size(); i++) {
		delete _materials[i];
	}
}
ModelData::~ModelData() {
	ClearModel();
}