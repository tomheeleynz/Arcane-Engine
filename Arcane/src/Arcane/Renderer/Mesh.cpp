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
				SubMesh* newSubmesh = new SubMesh(currentMesh);
				m_SubMeshes.push_back(newSubmesh);
			}
		}

	}
}
