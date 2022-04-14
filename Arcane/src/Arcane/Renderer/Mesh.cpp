#include <iostream>
#include "Mesh.h"

namespace Arcane
{
	Mesh::Mesh(std::string filepath)
	{
		Assimp::Importer importer;
		
		const aiScene* scene = importer.ReadFile(
			"D:\\Engine-Development\\ArcaneEngine\\EnchantingTable\\src\\Assets\\Models\\Mandalorian.fbx",
			aiProcess_Triangulate 
		);

		if (!scene) {
			std::cout << importer.GetErrorString() << std::endl;
		}
		else {
			// Process Mesh into Submeshes
			for (int i = 0; i < scene->mNumMeshes; i++) {
				aiMesh* currentMesh = scene->mMeshes[i];

				for (int j = 0; j < currentMesh->mNumVertices; j++) {
					MeshVertex newVertex;
					
					newVertex.vertex = {
						currentMesh->mVertices[j].x,
						currentMesh->mVertices[j].y,
						currentMesh->mVertices[j].z
					};

					newVertex.normal = {
						currentMesh->mNormals[j].x,
						currentMesh->mNormals[j].y,
						currentMesh->mNormals[j].z
					};

					newVertex.texture = {
						currentMesh->mTextureCoords[0][j].x,
						currentMesh->mTextureCoords[0][j].y
					};

					m_Vertices.push_back(newVertex);
				}

				for (int k = 0; k < currentMesh->mNumFaces; k++) {
					aiFace face = currentMesh->mFaces[k];
					for (int j = 0; j < face.mNumIndices; j++) {
						m_Indices.push_back(face.mIndices[j]);
					}
				}
			}
		}

		m_VertexBuffer = VertexBuffer::Create(m_Vertices.data(), sizeof(MeshVertex) * m_Vertices.size());
		m_IndexBuffer = IndexBuffer::Create(m_Indices.data(), m_Indices.size());
		m_VertexBuffer->AddIndexBuffer(m_IndexBuffer);
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
