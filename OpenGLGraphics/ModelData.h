#pragma once
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
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
	bool LoadModel(const std::string& fileName); // Load model from file
	void RenderModel();
	void ClearModel();
private:
	bool LoadNode(aiNode* node, const aiScene* scene);
	bool LoadMaterials(const aiScene* scene);
	bool LoadTextures(aiTexture* texture);
	bool LoadMesh(aiMesh* mesh, const aiScene* scene);
	//Vertex Data
	std::vector<MeshData> _meshes;
	std::vector<Materials> _materials;
	std::vector<Texture> _textures;
	std::vector<size_t> _materialsIndx;
	std::vector<size_t> _texturesIndx;

	const aiScene* _scene;

};

