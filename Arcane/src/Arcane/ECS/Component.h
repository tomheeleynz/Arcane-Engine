#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Arcane/Renderer/Mesh.h"

namespace Arcane 
{
	enum class LightType
	{
		DIRECTIONAL,
		POINT
	};

	struct TagComponent
	{
		std::string tag;
	};

	struct TransformComponent
	{
		glm::vec3 pos = {0.0f, 0.0f, 0.0f};
		glm::quat rotation = {0.0, 0.0, 0.0, 1.0};
		glm::vec3 scale = {1.0f, 1.0f, 1.0f };
	};

	struct MeshComponent
	{
		Mesh* mesh = nullptr;
		std::string filepath;
	};

	struct MeshRendererComponent
	{
		Material* material;
	};

	struct LightComponent
	{
		LightType type;
		glm::vec3 color;
	};
}