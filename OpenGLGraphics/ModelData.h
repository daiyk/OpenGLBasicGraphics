#pragma once
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "MeshData.h"
#include "Materials.h"
#include "Texture.h"
class ModelData
{
public:
	ModelData();
	bool LoadModel(const std::string& fileName);
	void RenderModel();
	void ClearModel();
private:
	//Vertex Data
	std::vector<MeshData> _meshes;
	std::vector<Materials> _materials;
	std::vector<Texture> _textures;
	std::vector<size_t> _materialsIndx;
	std::vector<size_t> _texturesIndx;

};

