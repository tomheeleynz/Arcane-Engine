#include "ScenePanel.h"
#include "Arcane.h"

ScenePanel::ScenePanel()
{
	m_SelectedEntity = {};
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

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectedEntity = {};

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				m_Context->CreateEntity("Empty Entity");
			
			if (ImGui::BeginMenu("3D Object"))
			{
				if (ImGui::MenuItem("Plane")) {
					CreateMeshEntity(MeshEntityType::PLANE);
				}
				
				if (ImGui::MenuItem("Cube")) {
					std::cout << "Creating a Cube\n";
				}
				
				ImGui::EndMenu();
			}
			
			ImGui::EndPopup();
		}

	}
	ImGui::End();

}

void ScenePanel::DrawNode(Arcane::Entity& entity) 
{
	// Get Tag Component of Entity
	std::string tag = entity.GetComponent<Arcane::TagComponent>().tag;
	
	ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	
	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
	if (ImGui::IsItemClicked())
	{
		m_SelectedEntity = entity;
	}

	if (opened)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
		if (opened)
			ImGui::TreePop();
		ImGui::TreePop();
	}


}

Arcane::Entity& ScenePanel::CreateMeshEntity(MeshEntityType type)
{
	// Create Entity
	Arcane::Entity* entity = nullptr;
	
	// Create Mesh Component
	Arcane::MeshComponent meshComponent;

	// Create Mesh Renderer Component
	Arcane::MeshRendererComponent meshRendererComponent;
	meshRendererComponent.material = nullptr;

	// Select what type of mesh to give to
	// Entity
	switch (type)
	{
	case ScenePanel::MeshEntityType::PLANE:
	{
		entity = m_Context->CreateEntity("Plane");
		meshComponent.mesh = Arcane::MeshFactory::CreatePlane();
		break;
	}
	case ScenePanel::MeshEntityType::CUBE:
	{
		// Create a cube at some point
		break;
	} 
	default:
		break;
	}

	// Add Components to entity
	entity->AddComponent<Arcane::MeshComponent>(meshComponent);
	entity->AddComponent<Arcane::MeshRendererComponent>(meshRendererComponent);

	return *entity;
}
