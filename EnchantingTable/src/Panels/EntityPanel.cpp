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

	if (entity.HasComponent<MeshComponent>()) 
	{
		DrawComponent<MeshComponent>("Mesh", entity, [](auto& component) {
		
		});
	}

	if (entity.HasComponent<MeshRendererComponent>())
	{
		DrawComponent<MeshRendererComponent>("Mesh Renderer", entity, [](auto& component) {
			Material* material = component.material;
			for (Arcane::ShaderVariable variable : material->GetMaterialVariables())
			{
				if (variable.Type == Arcane::ShaderVariableType::Vec3)
				{
					glm::vec3 currentValue = material->GetVec3(variable.offset);

					if (currentValue.x < 0)
						currentValue = {0.0f, 0.0f, 0.0f};

					ImGui::ColorEdit3(variable.Name.c_str(), glm::value_ptr(currentValue));
					material->WriteVec3(variable.offset, currentValue);
				}
			}
		});
	}

	if (entity.HasComponent<Light>())
	{
		DrawComponent<Light>("Light", entity, [](auto& component) {
			if (component.Type == LightType::DIRECTIONAL) {
				ImGui::Text("Directional");
			}

			glm::vec3 currentValue = component.color;
			ImGui::ColorEdit3("Color", glm::value_ptr(currentValue));
			component.color = currentValue;
		});
	}
}

