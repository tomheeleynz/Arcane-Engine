#pragma once

#include <string>
#include <assimp/Importer.hpp>      
#include <assimp/scene.h>          
#include <assimp/postprocess.h>     

#include "SubMesh.h"

namespace Arcane
{
	class Mesh
	{
	public:
		Mesh(std::string filepath);

	private:
		std::vector<SubMesh*> m_SubMeshes;
	};
}