#include "EntityPanel.h"
#include "PanelStructs.h"


template <typename T, typename UIFunction>
static void DrawComponent(std::string name, Arcane::Entity entity, UIFunction uiFunction)
{
	using namespace Arcane;
	if (entity.HasComponent<T>())
	{
		ImGui::Text(name.c_str());
		auto& component = entity.GetComponent<T>();
		uiFunction(component);
		ImGui::Separator();
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

	DrawComponent<TagComponent>("Tag", entity, [](auto& component) {
		auto& tag = component.tag;

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		std::strncpy(buffer, tag.c_str(), sizeof(buffer));
		if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
		{
			tag = std::string(buffer);
		}
	});


	DrawComponent<TransformComponent>("Transform", entity, [](auto& component) {
		ImGui::InputFloat3("Position", glm::value_ptr(component.pos));
		ImGui::InputFloat3("Rotation", glm::value_ptr(component.rotation));
		ImGui::InputFloat3("Scale", glm::value_ptr(component.scale));
	});


	DrawComponent<MeshComponent>("Mesh", entity, [](auto& component) {
		// Create something i can add to 
		ImGui::Text("Mesh");
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CURRENT_SELECTED_ASSET");

			if (payload != nullptr) {
				// Get Asset Id
				AssetInfo assetInfo = *static_cast<AssetInfo*>(payload->Data);
				Asset* meshAsset = Application::Get().GetAssetDatabase().GetAsset(assetInfo.id);

				if (meshAsset != nullptr && meshAsset->GetAssetType() == AssetType::MESH) {
					Mesh* mesh = static_cast<Mesh*>(meshAsset);
					component.mesh = mesh;
				}
			}
			ImGui::EndDragDropTarget();
		}
	});
	


	DrawComponent<MeshRendererComponent>("Mesh Renderer", entity, [this](auto& component) {
		ImGui::Text("Material");

		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CURRENT_SELECTED_ASSET");

			if (payload != nullptr) {
				// Get Asset Id
				AssetInfo assetInfo = *static_cast<AssetInfo*>(payload->Data);
				Asset* materialAsset = Application::Get().GetAssetDatabase().GetAsset(assetInfo.id);

				if (materialAsset != nullptr && materialAsset->GetAssetType() == AssetType::MATERIAL) {
					Material* material = static_cast<Material*>(materialAsset);
					component.material = material;
				}
			}
			ImGui::EndDragDropTarget();
		}

		if (component.material != nullptr) { 
			m_SelectedMaterial = component.material;
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

	DrawComponent<ScriptComponent>("Script", entity, [](auto& component) {
		ImGui::Text("Load Script");
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CURRENT_SELECTED_ASSET");

			if (payload != nullptr) {
				// Get Asset Id
				AssetInfo assetInfo = *static_cast<AssetInfo*>(payload->Data);
				Asset* scriptAsset = Application::Get().GetAssetDatabase().GetAsset(assetInfo.id);

				if (scriptAsset != nullptr && scriptAsset->GetAssetType() == AssetType::SCRIPT) {
					Script* script = static_cast<Script*>(scriptAsset);
					component.script = script;
				}
			}
			ImGui::EndDragDropTarget();
		}
	});

	DrawComponent<RigidBodyComponent>("Rigid Body", entity, [](auto& component) {
		float currentMass = component.rigidBody->GetMass();
		ImGui::InputFloat("Mass", &currentMass);
		component.rigidBody->SetMass(currentMass);
	});

	ImGui::SameLine();
	ImGui::PushItemWidth(-1);

	if (ImGui::Button("Add Component"))
		ImGui::OpenPopup("AddComponent");

	if (ImGui::BeginPopup("AddComponent"))
	{
		DisplayAddComponentEntry<MeshComponent>("Mesh");
		DisplayAddComponentEntry<MeshRendererComponent>("Mesh Renderer");
		DisplayAddComponentEntry<RigidBodyComponent>("Rigid Body");
		DisplayAddComponentEntry<ScriptComponent>("Script");
		DisplayAddComponentEntry<BoxColliderComponent>("Box Collider");
		ImGui::EndPopup();
	}

	ImGui::PopItemWidth();

	
}

template<typename T>
void EntityPanel::DisplayAddComponentEntry(std::string entryName)
{
	using namespace Arcane;
	
	if (!m_Context.HasComponent<T>())
	{
		if (ImGui::MenuItem(entryName.c_str()))
		{
			InitComponent<T>();
			ImGui::CloseCurrentPopup();
		}
	}
}

template <>
void EntityPanel::InitComponent<Arcane::RigidBodyComponent>() 
{
	Arcane::RigidBodyComponent component;
	Arcane::TransformComponent& transformComponent = m_Context.GetComponent<Arcane::TransformComponent>();

	Arcane::RigidBody* rigidBody = new Arcane::RigidBody(transformComponent.pos);
	component.rigidBody = rigidBody;

	m_Context.GetScene()->AddToPhyicsScene(rigidBody->GetRigidBody());
	
	m_Context.AddComponent<Arcane::RigidBodyComponent>(component);
}

template <>
void EntityPanel::InitComponent<Arcane::BoxColliderComponent>()
{
	glm::vec3 min = glm::vec3();
	glm::vec3 max = glm::vec3();

	if (m_Context.HasComponent<Arcane::MeshComponent>())
	{
		auto& meshComponent = m_Context.GetComponent<Arcane::MeshComponent>();
		
		min = {
			meshComponent.mesh->GetBoundingBox().bbMin.x, 
			meshComponent.mesh->GetBoundingBox().bbMin.y, 
			meshComponent.mesh->GetBoundingBox().bbMin.z 
		};
		
		max = {
			meshComponent.mesh->GetBoundingBox().bbMax.x,
			meshComponent.mesh->GetBoundingBox().bbMax.y,
			meshComponent.mesh->GetBoundingBox().bbMax.z
		};
	}

	auto& rigidBodyComponent = m_Context.GetComponent<Arcane::RigidBodyComponent>();

	Arcane::BoxColliderComponent newComponent;
	newComponent.boxCollider = new Arcane::BoxCollider(min, max, rigidBodyComponent.rigidBody->GetRigidBody());

	m_Context.AddComponent<Arcane::BoxColliderComponent>(newComponent);
}

template <>
void EntityPanel::InitComponent<Arcane::ScriptComponent>()
{
	m_Context.AddComponent<Arcane::ScriptComponent>();
}

template <>
void EntityPanel::InitComponent<Arcane::MeshComponent>()
{
	m_Context.AddComponent<Arcane::MeshComponent>();
}

template <>
void EntityPanel::InitComponent<Arcane::MeshRendererComponent>()
{
	m_Context.AddComponent<Arcane::MeshRendererComponent>();
}


