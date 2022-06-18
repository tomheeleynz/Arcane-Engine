#include <iostream>
#include "Mesh.h"

namespace Arcane
{
	static const uint32_t s_MeshImportFlags = aiProcess_Triangulate | aiProcess_FlipUVs;

	Mesh::Mesh(std::string filepath)
	{
		Assimp::Importer importer;
		
		const aiScene* scene = importer.ReadFile(
			"D:\\Engine-Development\\ArcaneEngine\\EnchantingTable\\src\\Assets\\Models\\Backpack.fbx", s_MeshImportFlags
		);

		if (!scene) {
			std::cout << importer.GetErrorString() << std::endl;
		}
		else {
			ProcessNode(scene->mRootNode, scene);
		}

		m_VertexBuffer = VertexBuffer::Create(m_Vertices.data(), sizeof(MeshVertex) * m_Vertices.size());
		m_IndexBuffer = IndexBuffer::Create(m_Indices.data(), m_Indices.size());
		m_VertexBuffer->AddIndexBuffer(m_IndexBuffer);
	}

	void Mesh::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			ProcessMesh(mesh, scene);
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			ProcessNode(node->mChildren[i], scene);
		}
	}


	void Mesh::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		// Process Mesh into Submeshes
		for (int i = 0; i < mesh->mNumVertices; i++) {
			MeshVertex newVertex;

			newVertex.vertex = {
				mesh->mVertices[i].x,
				mesh->mVertices[i].y,
				mesh->mVertices[i].z
			};

			newVertex.normal = {
				mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z
			};

			if (mesh->mTextureCoords[0])
			{
				newVertex.texture = {
					mesh->mTextureCoords[0][i].x,
					mesh->mTextureCoords[0][i].y
				};
			}
			else {
				newVertex.texture = {0.0f, 0.0f};
			}

			m_Vertices.push_back(newVertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];

			for (unsigned j = 0; j < face.mNumIndices; j++) {
				m_Indices.push_back(face.mIndices[j]);
			}
		}

	}

	Material* Mesh::GetMaterial()
	{
		return m_Material;
	}

	void Mesh::SetMaterial(Material* material) 
	{
		m_Material = material;
	}

}
