#include "EntityPanel.h"

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
			std::string entityName = m_Context.GetComponent<Arcane::TagComponent>().tag;
			if (ImGui::TreeNode(entityName.c_str())) 
			{
			
				ImGui::TreePop();
			}
		}
	}
	ImGui::End();
}