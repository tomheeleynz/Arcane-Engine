#pragma once

#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>

#include "Arcane/Renderer/Material.h"

namespace Arcane
{
	class MaterialDeserializer 
	{
	public:
		MaterialDeserializer(std::filesystem::path path);

		Material* Deserialize();
	private:
		std::filesystem::path m_Path;
	};
}