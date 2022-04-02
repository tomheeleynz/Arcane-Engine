#include <iostream>
#include "Mesh.h"

namespace Arcane
{
	Mesh::Mesh(std::string filepath)
	{
		Assimp::Importer importer;
		
		const aiScene* scene = importer.ReadFile(".\\src\\Assets\\Models\\Mandalorian.fbx",
			aiProcess_Triangulate 
		);

		if (!scene) {
			std::cout << importer.GetErrorString() << std::endl;
		}
	}
}
