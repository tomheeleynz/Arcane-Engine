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
		if (ImGui::Button("Add Entity"))
			CreateMeshEntity(MeshEntityType::PLANE);
	
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
		m_SelectedEntity = entity;
		ImGui::TreePop();
	}
}

Arcane::Entity& ScenePanel::CreateMeshEntity(MeshEntityType type)
{
	// Get Default Material
	Arcane::Asset* materialAsset = Arcane::Application::Get().GetAssetDatabase().GetDefaultAsset("MeshMaterial");
	Arcane::Material* material = static_cast<Arcane::Material*>(materialAsset);

	// Create Entity
	Arcane::Entity* entity = m_Context->CreateEntity("New Entity");
	
	// Create Mesh Component
	Arcane::MeshComponent meshComponent;

	// Create Mesh Renderer Component
	Arcane::MeshRendererComponent meshRendererComponent;
	meshRendererComponent.material = material;

	// Select what type of mesh to give to
	// Entity
	switch (type)
	{
	case ScenePanel::MeshEntityType::PLANE:
	{
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
