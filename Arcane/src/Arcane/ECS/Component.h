#pragma once

#include <string>
#include <glm/glm.hpp>

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