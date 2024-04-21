#pragma once
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <filesystem>
#include "MeshData.h"
#include "Materials.h"
#include "Texture.h"
/// <summary>
/// This modeldata class uses assimp library to load models from file
/// </summary>
class ModelData
{
public:
	ModelData();
	bool LoadModel(const std::string& fileName); // Load model from file. only support local path
	void RenderModel();
	void ClearModel();
private:
	bool LoadNode(aiNode* node, const aiScene* scene);
	bool LoadMaterials(const aiScene* scene);
	bool LoadTextures(aiMaterial* material, aiTextureType type); //helper function to load textures
	bool LoadMesh(aiMesh* mesh, const aiScene* scene);
	//Vertex Data
	std::vector<MeshData> _meshes;
	std::vector<Materials> _materials;
	std::vector<Texture> _textures;
	std::vector<size_t> _meshTomaterialsIndx;
	std::vector<size_t> _texturesIndx;
	
	std::filesystem::path _modelPath;
	const aiScene* _scene;

};

