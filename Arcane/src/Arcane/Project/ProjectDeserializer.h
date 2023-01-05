#pragma once

#include <filesystem>
#include <nlohmann/json.hpp>
#include <fstream>
#include "Project.h"

namespace Arcane
{
	class ProjectDeserializer
	{
	public:
		ProjectDeserializer(std::filesystem::path path);

		Project* Deserialize();
	private:
		std::filesystem::path m_Path;
	};
}