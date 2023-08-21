#include <string>

#include "SceneSerializer.h"
#include "Arcane/ECS/Entity.h"
#include "Arcane/Renderer/MaterialSerializer.h"

namespace Arcane
{
	SceneSerializer::SceneSerializer(Scene* scene)
	{
		m_Scene = scene;
	}

	void SceneSerializer::Serialize(std::string filepath)
	{
		// Setup json object
		std::filesystem::path savePath(filepath);
		nlohmann::json jsonObject;

		if (m_Scene == nullptr) {
			jsonObject["name"] = savePath.stem().string();
		}
		else {
			// Set Name of scene
			jsonObject["name"] = m_Scene->GetName();

			nlohmann::json entityArray = nlohmann::json::array();
			nlohmann::json entityObject = nlohmann::json::object();

			//// Iterate through all entities and save them
			m_Scene->m_Registry.each([&](auto entity) {
				Entity sceneEntity(entity, m_Scene);

				if (sceneEntity.HasComponent<TagComponent>())
				{
					std::string tag = sceneEntity.GetComponent<TagComponent>().tag;
					entityObject["name"] = tag;
				}

				nlohmann::json componentArray = nlohmann::json::array();

				if (sceneEntity.HasComponent<TransformComponent>())
				{
					nlohmann::json transformObject = nlohmann::json::object();
					transformObject["name"] = "Transform";

					TransformComponent component = sceneEntity.GetComponent<TransformComponent>();

					nlohmann::json position = { component.pos.x, component.pos.y, component.pos.z };
					nlohmann::json rotation = { component.rotation.x, component.rotation.y, component.rotation.z };
					nlohmann::json scale = { component.scale.x, component.scale.y, component.scale.z };

					transformObject["position"] = position;
					transformObject["rotation"] = rotation;
					transformObject["scale"] = scale;

					componentArray.push_back(transformObject);
				}

				if (sceneEntity.HasComponent<CameraComponent>())
				{
					nlohmann::json cameraObject = nlohmann::json::object();
					cameraObject["name"] = "Camera";

					CameraComponent& cameraComponent = sceneEntity.GetComponent<CameraComponent>();
					cameraObject["isPrimary"] = cameraComponent.isPrimary;
					cameraObject["cameraType"] = cameraComponent.type == CameraType::Perspective ? 0 : 1;
					componentArray.push_back(cameraObject);
				}

				if (sceneEntity.HasComponent<MeshComponent>())
				{
					MeshComponent component = sceneEntity.GetComponent<MeshComponent>();
					nlohmann::json meshObject = nlohmann::json::object();

					meshObject["name"] = "Mesh";
					meshObject["AssetID"] = component.mesh->GetID();

					componentArray.push_back(meshObject);
				}

				if (sceneEntity.HasComponent<MeshRendererComponent>())
				{
					MeshRendererComponent component = sceneEntity.GetComponent<MeshRendererComponent>();
					nlohmann::json meshRendererObject = nlohmann::json::object();

					meshRendererObject["name"] = "MeshRenderer";
					meshRendererObject["AssetID"] = component.material->GetID();

					// Need to serialize materil
					Arcane::MaterialSerializer serialize(component.material);
					serialize.Serialize(component.material->GetPath().string());

					componentArray.push_back(meshRendererObject);
				}

				if (sceneEntity.HasComponent<LightComponent>())
				{
					LightComponent& component = sceneEntity.GetComponent<LightComponent>();
					nlohmann::json lightObject = nlohmann::json::object();
					lightObject["name"] = "Light";
					lightObject["type"] = component.type;
					lightObject["color"] = { component.color.x, component.color.y, component.color.z };
					componentArray.push_back(lightObject);
				}

				if (sceneEntity.HasComponent<RigidBodyComponent2D>())
				{
					RigidBodyComponent2D& rigidBodyComponent2D = sceneEntity.GetComponent<RigidBodyComponent2D>();
					nlohmann::json rigidBodyObject2D = nlohmann::json::object();
					
					rigidBodyObject2D["name"] = "RigidBody2D";
					rigidBodyObject2D["gravityScale"] = rigidBodyComponent2D.gravityScale;
					rigidBodyObject2D["mass"] = rigidBodyComponent2D.mass;
					
					componentArray.push_back(rigidBodyObject2D);
				}

				if (sceneEntity.HasComponent<SpriteRendererComponent>())
				{
					SpriteRendererComponent& spriteRendererComponent = sceneEntity.GetComponent<SpriteRendererComponent>();
					nlohmann::json spriteRendererObject = nlohmann::json::object();

					spriteRendererObject["name"] = "SpriteRenderer";
					if (spriteRendererComponent.sprite == nullptr)
						spriteRendererObject["sprite"] = -1;
					else
						spriteRendererObject["sprite"] = spriteRendererComponent.sprite->GetID();

					spriteRendererObject["color"] = {
						spriteRendererComponent.color.x,
						spriteRendererComponent.color.y,
						spriteRendererComponent.color.z
					};

					componentArray.push_back(spriteRendererObject);
				}

				if (sceneEntity.HasComponent<Animator>())
				{
					Animator& animator = sceneEntity.GetComponent<Animator>();
					nlohmann::json animatorObject = nlohmann::json::object();
					animatorObject["name"] = "Animator";

					if (animator.controller == nullptr)
						animatorObject["controller"] = -1;
					else
						animatorObject["controller"] = animator.controller->GetID();

					componentArray.push_back(animatorObject);
				}

				entityObject["Components"] = componentArray;
				entityArray.push_back(entityObject);
			});

			jsonObject["Entities"] = entityArray;
		}


		// Save json objet to file
		std::ofstream o(savePath.string());
		o << std::setw(4) << jsonObject << std::endl;
	}
}