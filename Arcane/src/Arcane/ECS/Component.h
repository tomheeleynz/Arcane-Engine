#pragma once

#include <string>
#include <glm/glm.hpp>

#include "Arcane/Renderer/Mesh.h"

namespace Arcane 
{
	struct TagComponent
	{
		std::string tag;
	};

	struct TransformComponent
	{
		glm::vec3 pos;
		glm::vec3 rot;
		glm::vec3 scale;
	};

	struct MeshComponent
	{
		Mesh* mesh;
		std::string filepath;
	};
}