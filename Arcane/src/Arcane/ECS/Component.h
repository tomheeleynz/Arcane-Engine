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
		glm::vec3 pos = {0.0f, 0.0f, 0.0f};
		glm::vec3 rot = {0.0f, 0.0f, 0.0f};
		glm::vec3 scale = {1.0f, 1.0f, 1.0f };
	};

	struct MeshComponent
	{
		Mesh* mesh;
		std::string filepath;
	};

	struct MeshRendererComponent
	{

	};
}