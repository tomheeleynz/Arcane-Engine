#include <iostream>

#include "SceneDeserializer.h"
#include "Arcane/ECS/Component.h"
#include "Arcane/Renderer/Mesh.h"
#include "Arcane/ECS/Entity.h"
#include "Arcane/Core/Application.h"

namespace Arcane
{
	SceneDeserializer::SceneDeserializer(std::string filepath, bool file)
	{
		if (file)
			m_Filepath = filepath;
		else
			m_JSONString = filepath;

		m_IsFile = true;
	}

	Scene* SceneDeserializer::Deserialize()
	{
		Scene* newScene = new Scene(false);

		// Read Json File
		nlohmann::json jsonObject;
		
		if (m_IsFile) {
			std::ifstream i(m_Filepath);
			i >> jsonObject;
		}
		else {
			jsonObject = nlohmann::json::parse(m_JSONString);
		}

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
					newComponent.pos = { element["position"][0], element["position"][1], element["position"][2] };
					newComponent.scale = { element["scale"][0], element["scale"][1], element["scale"][2] };

					newEntity->AddComponent<TransformComponent>(newComponent);
				}
				else if (name == "Camera")
				{
					CameraComponent newComponent;
					newComponent.isPrimary = element["isPrimary"];
					newComponent.type = element["cameraType"] == 0 ? CameraType::Perspective : CameraType::Orthographic;
					newEntity->AddComponent<CameraComponent>(newComponent);
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

					light.color = { element["color"][0], element["color"][1], element["color"][2] };
					newEntity->AddComponent<LightComponent>(light);
				}
				else if (name == "RigidBody")
				{
					Kinetics::BodyDef bodyDef;
					RigidBodyComponent rigidBodyComponent;

					float gravityScale = element["gravityScale"];
					float mass = element["mass"];

					rigidBodyComponent.mass = mass;
					rigidBodyComponent.gravityScale = gravityScale;

					bodyDef.mass = mass;

					rigidBodyComponent.body = newScene->AddDynamicBodyToPhysicsWorld(bodyDef);
					rigidBodyComponent.body->SetGravityScale(gravityScale);
					newEntity->AddComponent<RigidBodyComponent>(rigidBodyComponent);
				}
				else if (name == "SpriteRenderer")
				{
					SpriteRendererComponent spriteRendererComponent;

					spriteRendererComponent.color = {
						element["color"][0],
						element["color"][1],
						element["color"][2]
					};

					if (element["sprite"] == -1)
						spriteRendererComponent.sprite = nullptr;
					else
						spriteRendererComponent.sprite = static_cast<Texture*>(Application::Get().GetAssetDatabase().GetAsset(element["sprite"]));

					newEntity->AddComponent<SpriteRendererComponent>(spriteRendererComponent);
				}
				else if (name == "Animator")
				{
					Animator animator;

					if (element["controller"] == -1)
						animator.controller = nullptr;
					else
						animator.controller = static_cast<AnimationController*>(Application::Get().GetAssetDatabase().GetAsset(element["controller"]));

					newEntity->AddComponent<Animator>(animator);
				}
			}
		}
	
		return newScene;
	}

	Scene* SceneDeserializer::Deserialize(nlohmann::json jsonObject)
	{
		Scene* newScene = new Scene(false);

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
					newComponent.pos = { element["position"][0], element["position"][1], element["position"][2] };
					newComponent.scale = { element["scale"][0], element["scale"][1], element["scale"][2] };

					newEntity->AddComponent<TransformComponent>(newComponent);
				}
				else if (name == "Camera")
				{
					CameraComponent newComponent;
					newComponent.isPrimary = element["isPrimary"];
					newComponent.type = element["cameraType"] == 0 ? CameraType::Perspective : CameraType::Orthographic;
					newEntity->AddComponent<CameraComponent>(newComponent);
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

					light.color = { element["color"][0], element["color"][1], element["color"][2] };
					newEntity->AddComponent<LightComponent>(light);
				}
				else if (name == "RigidBody")
				{
					Kinetics::BodyDef bodyDef;
					RigidBodyComponent rigidBodyComponent;

					float gravityScale = element["gravityScale"];
					float mass = element["mass"];

					rigidBodyComponent.mass = mass;
					rigidBodyComponent.gravityScale = gravityScale;

					bodyDef.mass = mass;

					rigidBodyComponent.body = newScene->AddDynamicBodyToPhysicsWorld(bodyDef);
					rigidBodyComponent.body->SetGravityScale(gravityScale);
					newEntity->AddComponent<RigidBodyComponent>(rigidBodyComponent);
				}
				else if (name == "SpriteRenderer")
				{
					SpriteRendererComponent spriteRendererComponent;

					spriteRendererComponent.color = {
						element["color"][0],
						element["color"][1],
						element["color"][2]
					};

					if (element["sprite"] == -1)
						spriteRendererComponent.sprite = nullptr;
					else
						spriteRendererComponent.sprite = static_cast<Texture*>(Application::Get().GetAssetDatabase().GetAsset(element["sprite"]));

					newEntity->AddComponent<SpriteRendererComponent>(spriteRendererComponent);
				}
				else if (name == "Animator")
				{
					Animator animator;

					if (element["controller"] == -1)
						animator.controller = nullptr;
					else
						animator.controller = static_cast<AnimationController*>(Application::Get().GetAssetDatabase().GetAsset(element["controller"]));

					newEntity->AddComponent<Animator>(animator);
				}
			}
		}

		return newScene;
	}
}