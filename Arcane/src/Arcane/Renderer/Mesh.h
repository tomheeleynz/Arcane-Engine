#pragma once

#include <glm/glm.hpp>
#include <string>
#include <assimp/Importer.hpp>      
#include <assimp/scene.h>          
#include <assimp/postprocess.h>     

#include "Material.h"
#include "SubMesh.h"

namespace Arcane
{
	class Mesh
	{
	public:
		Mesh(std::string filepath);
		Mesh(std::vector<MeshVertex> vertices, std::vector<uint32_t> indices);

		std::vector<SubMesh*> GetSubMeshes() { return m_SubMeshes; }
	private:
		void ProcessNode(aiNode* node, const aiScene* scene);
		SubMesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
	private:
		std::vector<SubMesh*> m_SubMeshes;
	};
}