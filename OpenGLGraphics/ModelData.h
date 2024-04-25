#pragma once
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <filesystem>
#include "MeshData.h"
#include "Materials.h"
#include "Texture.h"
#include "Shader.h"
/// <summary>
/// This modeldata class uses assimp library to load models from file
/// </summary>
class ModelData
{
public:
	ModelData();
	~ModelData();
	bool LoadModel(const std::string fileName); // Load model from file. only support local path
	void RenderModel(Shader &shader);
	void ClearModel();
private:
	bool LoadNode(aiNode* node, const aiScene* scene);
	bool LoadMaterials(const aiScene* scene);
	bool LoadTextures(aiMaterial* material, aiTextureType type, std::vector<size_t> &indices); //helper function to load textures from assimp package
	bool LoadMesh(aiMesh* mesh, const aiScene* scene);

	//Vertex Data, persist with the model object
	std::vector<MeshData*> _meshes;
	std::vector<Materials*> _materials;
	std::vector<Texture*> _textures;
	std::vector<size_t> _meshTomaterialsIndx;
	
	std::filesystem::path _modelPath;

};

