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
		Scene* newScene = new Scene();

		// Read Json File
		nlohmann::json jsonObject;
		std::ifstream i(m_Filepath);
		i >> jsonObject;

		// Set name of scene
		newScene->SetName(jsonObject["name"]);

		



		return nullptr;
	}
}