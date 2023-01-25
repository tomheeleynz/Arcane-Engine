#include "MaterialDeserializer.h"

namespace Arcane
{
	MaterialDeserializer::MaterialDeserializer(std::filesystem::path path)
	{
		m_Path = path;
	}

	Material* MaterialDeserializer::Deserialize()
	{
		// Reading in material json 
		nlohmann::json jsonObject;
		std::ifstream i(m_Path.string());
		i >> jsonObject;

		// Get name of material
		std::string name = jsonObject["name"];

		return Material::Create(nullptr);
	}
}