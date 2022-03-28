#include <iostream>
#include "SceneDeserializer.h"

namespace Arcane
{
	SceneDeserializer::SceneDeserializer(std::string filepath)
	{
		m_Filepath = filepath;
	}

	Scene* SceneDeserializer::Deserialize()
	{
		nlohmann::json jsonObject;
		std::ifstream i(m_Filepath);
		i >> jsonObject;

		// Parse Actual File
		std::cout << jsonObject["name"] << std::endl;

		return nullptr;
	}
}