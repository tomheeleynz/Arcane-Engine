#include "EntityPanel.h"


template <typename T, typename UIFunction>
static void DrawComponent(std::string name, Arcane::Entity entity, UIFunction uiFunction)
{
	using namespace Arcane;
	if (entity.HasComponent<T>())
	{
		ImGui::Text(name.c_str());
		auto& component = entity.GetComponent<T>();
		uiFunction(component);
	}
}


EntityPanel::EntityPanel()
{
	m_Context = {};
}

void EntityPanel::SetContext(Arcane::Entity& entity)
{
	m_Context = entity;
}

void EntityPanel::Update() 
{
	ImGui::Begin("Entity");
	{
		if (ImGui::Button("Add Component")) {
			AddMeshComponent(m_Context);
		}

		if (m_Context) 
		{
			DrawComponents(m_Context);
		}
	}
	ImGui::End();
}

void EntityPanel::DrawComponents(Arcane::Entity& entity)
{
	using namespace Arcane;

	if (entity.HasComponent<TagComponent>())
	{
		auto& tag = entity.GetComponent<TagComponent>().tag;

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		std::strncpy(buffer, tag.c_str(), sizeof(buffer));
		if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
		{
			tag = std::string(buffer);
		}
	}

	DrawComponent<TransformComponent>("Transform", entity, [](auto& component) {
		ImGui::InputFloat3("Position", glm::value_ptr(component.pos));
		ImGui::InputFloat3("Scale", glm::value_ptr(component.scale));
		ImGui::InputFloat3("Rotation", glm::value_ptr(component.rot));
	});


	DrawComponent<MeshComponent>("Mesh", entity, [](auto& component) {
		// Create something i can add to 
		ImGui::Text("Mesh");
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CURRENT_SELECTED_ASSET");

			if (payload != nullptr) {
				int assetID = *static_cast<int*>(payload->Data);
				MeshAsset* meshAsset = static_cast<MeshAsset*>(Arcane::Application::Get().GetAssetDatabase().GetAsset(assetID));
				meshAsset->LoadAsset();
				component.mesh = meshAsset->GetMesh();
			}
			ImGui::EndDragDropTarget();
		}
	});
	


	DrawComponent<MeshRendererComponent>("Mesh Renderer", entity, [](auto& component) {
		Material* material = component.material;

		if (material != nullptr) {
			for (Arcane::ShaderVariable variable : material->GetMaterialVariables())
			{
				if (variable.Type == Arcane::ShaderVariableType::Sampler)
				{
					ImGui::Text("Albedo");

					if (ImGui::BeginDragDropTarget())
					{
						const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CURRENT_SELECTED_ASSET");

						if (payload != nullptr) {
							int assetID = *static_cast<int*>(payload->Data);
							Asset* asset = Arcane::Application::Get().GetAssetDatabase().GetAsset(assetID);
							
							if (asset != nullptr && asset->GetAssetType() == AssetType::TEXTURE)
							{
								TextureAsset* textureAsset = static_cast<TextureAsset*>(asset);
								textureAsset->LoadAsset();
								material->WriteTexture(variable.binding, textureAsset->GetTexture());
							}
						}
						ImGui::EndDragDropTarget();
					}
				}
			}
		}
	});
	

	DrawComponent<LightComponent>("Light", entity, [](auto& component) {
		if (component.type == LightType::DIRECTIONAL) {
			ImGui::Text("Directional");
		}

		glm::vec3 currentValue = component.color;
		ImGui::ColorEdit3("Color", glm::value_ptr(currentValue));
		component.color = currentValue;
	});
	
}

void EntityPanel::AddMeshComponent(Arcane::Entity& entity)
{
	Arcane::MeshComponent meshComponent;
	Arcane::MeshRendererComponent meshRendererComponent;

	meshRendererComponent.material = Arcane::Material::Create(Arcane::ShaderLibrary::GetShader("Mesh"));
	entity.AddComponent<Arcane::MeshComponent>(meshComponent);
	entity.AddComponent<Arcane::MeshRendererComponent>(meshRendererComponent);
}

