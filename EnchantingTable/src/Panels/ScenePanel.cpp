#include "ScenePanel.h"
#include "Arcane.h"

ScenePanel::ScenePanel()
{
}

void ScenePanel::SetContext(Arcane::Scene* context)
{
	m_Context = context;
}

void ScenePanel::Update()
{
	ImGui::Begin("Scene");
	{
		m_Context->m_Registry.each([this](auto entity) {
			Arcane::Entity Entity(entity, this->m_Context);
			DrawNode(Entity);
		});
	}
	ImGui::End();
}

void ScenePanel::DrawNode(Arcane::Entity& entity) 
{
	// Get Tag Component of Entity
	std::string tag = entity.GetComponent<Arcane::TagComponent>().tag;

	if (ImGui::TreeNode(tag.c_str()))
	{
		ImGui::TreePop();
	}
}
