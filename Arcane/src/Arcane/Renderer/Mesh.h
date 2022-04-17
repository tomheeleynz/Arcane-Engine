#pragma once

#include <glm/glm.hpp>
#include <string>
#include <assimp/Importer.hpp>      
#include <assimp/scene.h>          
#include <assimp/postprocess.h>     

#include "Material.h"
#include "SubMesh.h"
#include "Buffer.h"

namespace Arcane
{
	struct MeshVertex
	{
		glm::vec3 vertex;
		glm::vec3 normal;
		glm::vec2 texture;
	};

	class Mesh
	{
	public:
		Mesh(std::string filepath);

		Material* GetMaterial();
		void SetMaterial(Material* material);

		VertexBuffer* GetVertexBuffer() { return m_VertexBuffer; }
	private:
		void ProcessNode(aiNode* node, const aiScene* scene);
		void ProcessMesh(aiMesh* mesh, const aiScene* scene);
	private:
		std::vector<MeshVertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
		
		VertexBuffer* m_VertexBuffer;
		IndexBuffer* m_IndexBuffer;
		
		Material* m_Material;
	};
}