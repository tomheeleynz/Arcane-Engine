#include <iostream>

#include "SceneDeserializer.h"
#include "Arcane/ECS/Component.h"
#include "Arcane/Renderer/Mesh.h"
#include "Arcane/ECS/Entity.h"
#include "Arcane/Core/Application.h"

namespace Arcane
{
	SceneDeserializer::SceneDeserializer(std::string filepath)
	{
		m_Filepath = filepath;
	}

	Scene* SceneDeserializer::Deserialize()
	{
		Scene* newScene = new Scene(false);

		// Read Json File
		nlohmann::json jsonObject;
		std::ifstream i(m_Filepath);
		i >> jsonObject;

		// Set name of scene
		newScene->SetName(jsonObject["name"]);
		AssetDatabase assetDatabase = Application::Get().GetAssetDatabase();

		for (auto& element : jsonObject["Entities"]) {
			std::string entityName = element["name"];
			Entity* newEntity = newScene->CreateEntity(entityName);

			for (auto& element : element["Components"]) {
				// Get Component Name
				std::string name = element["name"];

				if (name == "Transform") 
				{
					TransformComponent newComponent;
					newEntity->AddComponent<TransformComponent>(newComponent);
				}
				else if (name == "Mesh") 
				{
					uint64_t assetID = element["AssetID"];

					// Add a mesh component to hold the mesh
					MeshComponent newComponent;
					newComponent.mesh = static_cast<Mesh*>(assetDatabase.GetAsset(assetID));
					newEntity->AddComponent<MeshComponent>(newComponent);
				}
				else if (name == "MeshRenderer") {
					// Add a mesh render component to hold the material
					MeshRendererComponent newRendererComponent;
					uint64_t assetID = element["AssetID"];
					newRendererComponent.material = static_cast<Material*>(assetDatabase.GetAsset(assetID));
					newEntity->AddComponent<MeshRendererComponent>(newRendererComponent);
				}
				else if (name == "Light") {
					LightComponent light;

					int lightType = element["type"];

					if (lightType == 0) {
						light.type = LightType::DIRECTIONAL;
					}

					light.color = { element["color"][0], element["color"][1], element["color"][2]};
					newEntity->AddComponent<LightComponent>(light);
				}
			}
		}
	
		return newScene;
	}
}