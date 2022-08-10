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
					// Get Filepath of mesh
					std::string filepath = element["filepath"];
					
					// Add a mesh component to hold the mesh
					MeshComponent newComponent;
					newComponent.filepath = filepath;
					newComponent.mesh = new Mesh(filepath);
					newEntity->AddComponent<MeshComponent>(newComponent);

					// Add a mesh render component to hold the material
					MeshRendererComponent newRendererComponent;
					newRendererComponent.material = Material::Create(ShaderLibrary::GetShader("Mesh"));
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