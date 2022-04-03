#pragma once

#include <glm/glm.hpp>
#include <assimp/Importer.hpp>      
#include <assimp/scene.h>          
#include <assimp/postprocess.h>
#include <vector>

namespace Arcane
{
	struct SubMeshVertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texture;
	};

	class SubMesh
	{
	public:
		SubMesh(aiMesh* submesh);
	
	private:
		std::vector<SubMeshVertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
	};
}