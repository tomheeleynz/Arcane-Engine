#include <iostream>

#include "SceneDeserializer.h"
#include "Arcane/ECS/Component.h"
#include "Arcane/Renderer/Mesh.h"
#include "Arcane/ECS/Entity.h"

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

		for (auto& element : jsonObject["Entities"]) {
			std::string entityName = element["name"];
			Entity* newEntity = newScene->CreateEntity(entityName);

			for (auto& element : element["components"]) {
				// Get Component Name
				std::string name = element["name"];

				if (name == "Transform") 
				{
					TransformComponent newComponent;
					newEntity->AddComponent<TransformComponent>(newComponent);
				}
				else if (name == "Mesh") 
				{
					std::string filepath = element["filepath"];
					
					MeshComponent newComponent;
					newComponent.filepath = filepath;
					newComponent.mesh = new Mesh(filepath);

					newEntity->AddComponent<MeshComponent>(newComponent);
				}
			}
		}
	
		return newScene;
	}
}