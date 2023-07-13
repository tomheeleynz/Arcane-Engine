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
		uiFunction(component, entity);
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

	DrawComponent<TagComponent>("Tag", entity, [](auto& component, auto& entity) {
		auto& tag = component.tag;

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		std::strncpy(buffer, tag.c_str(), sizeof(buffer));
		if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
		{
			tag = std::string(buffer);
		}
	});


	DrawComponent<TransformComponent>("Transform", entity, [](auto& component, auto& entity) {
		// Position
		ImGui::InputFloat3("Position", glm::value_ptr(component.pos));

		// Rotation
		glm::vec3 rotation = glm::degrees(component.rotation);
		ImGui::InputFloat3("Rotation", glm::value_ptr(rotation));
		component.rotation = glm::radians(rotation);
		
		// Scale
		ImGui::InputFloat3("Scale", glm::value_ptr(component.scale));
	});

	DrawComponent<CameraComponent>("Camera", entity, [](auto& component, auto& entity) {
		ImGui::Checkbox("Is Primary:", &component.isPrimary);
		const char* items[] = { "Orthographic", "Perspective" };
		int chosenID = 0;

		// Display Combo box
		const char* combo_label = items[chosenID];

		if (ImGui::BeginCombo("Camera Type", combo_label))
		{
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				const bool is_selected = (chosenID == n);

				if (ImGui::Selectable(items[n], is_selected)) {
					chosenID = n;

					if (chosenID == 0) {
						component.type = CameraType::Orthographic;
					}
					else {
						component.type = CameraType::Perspective;
					}
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
	});

	DrawComponent<MeshComponent>("Mesh", entity, [](auto& component, auto& entity) {
		static char buf1[64] = "";
		ImGui::InputText("Mesh", buf1, 64);

		if (component.mesh != nullptr) {
			memset(buf1, 0, sizeof(buf1));
			std::strncpy(buf1, component.mesh->GetName().c_str(), sizeof(buf1));
		}

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
					memset(buf1, 0, sizeof(buf1));
					std::strncpy(buf1, meshAsset->GetName().c_str(), sizeof(buf1));
				}
			}
			ImGui::EndDragDropTarget();
		}
	});
	


	DrawComponent<MeshRendererComponent>("Mesh Renderer", entity, [this](auto& component, auto& entity) {
		static char buf1[64] = "";
		ImGui::InputText("Material", buf1, 64);

		if (component.material != nullptr) {
			memset(buf1, 0, sizeof(buf1));
			std::strncpy(buf1, component.material->GetName().c_str(), sizeof(buf1));
		}

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
					memset(buf1, 0, sizeof(buf1));
					std::strncpy(buf1, materialAsset->GetName().c_str(), sizeof(buf1));
				}
			}
			ImGui::EndDragDropTarget();
		}

		if (component.material != nullptr) { 
			m_SelectedMaterial = component.material;
		}
	});
	

	DrawComponent<LightComponent>("Light", entity, [](auto& component, auto& entity) {
		if (component.type == LightType::DIRECTIONAL) {
			ImGui::Text("Directional");
		}

		glm::vec3 currentValue = component.color;
		ImGui::ColorEdit3("Color", glm::value_ptr(currentValue));
		component.color = currentValue;
	});

	DrawComponent<ScriptComponent>("Script", entity, [this](auto& component, auto& entity) {
		static char buf1[64] = "";
		ImGui::InputText("Script", buf1, 64);

		if (component.script) {
			memset(buf1, 0, sizeof(buf1));
			std::strncpy(buf1, component.script->GetName().c_str(), sizeof(buf1));
		}

		if (component.script != nullptr) {
			ImGui::Text("Properties");
			ImGui::Text(component.updateProperties == true ? "Update" : "No Update");

			for (auto const& [key, val] : component.script->GetProperties()) {
				std::string type = val.type;

				if (type == "string") {
					std::string value = std::any_cast<std::string>(val.value);
				}

				if (type == "int") {
					int change = std::any_cast<int>(val.value);
					int value = std::any_cast<int>(val.value);
					ImGui::InputInt(key.c_str(), &change);

					if (change != value) {
						component.updateProperties = true;
						component.script->SetPropertyValue(key, change);
					}
				}

				if (type == "float") {
					float change = std::any_cast<float>(val.value);
					float value = std::any_cast<float>(val.value);
					ImGui::InputFloat(key.c_str(), &change);

					if (change != value) {
						component.updateProperties = true;
						component.script->SetPropertyValue(key, change);
					}
				}
			}
		}

		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CURRENT_SELECTED_ASSET");

			if (payload != nullptr) {
				// Get Asset Id
				AssetInfo assetInfo = *static_cast<AssetInfo*>(payload->Data);
				Asset* scriptAsset = Application::Get().GetAssetDatabase().GetAsset(assetInfo.id);

				if (scriptAsset != nullptr && scriptAsset->GetAssetType() == AssetType::SCRIPT) {
					Script* script = static_cast<Script*>(scriptAsset);
					script->SetEntity(entity);
					component.script = script;
					memset(buf1, 0, sizeof(buf1));
					std::strncpy(buf1, scriptAsset->GetName().c_str(), sizeof(buf1));
				}
			}
			ImGui::EndDragDropTarget();
		}
	});

	DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [this](auto& component, auto& entity) {
	});

	DrawComponent<RigidBodyComponent>("Rigid Body", entity, [this](auto& component, auto& entity) {
		ImGui::InputFloat("Gravity", &component.gravityScale);
		// Set dynamic body gravity scale

		ImGui::InputFloat("Mass", &component.mass);
		// set mass of component
	});

	DrawComponent<BoxColliderComponent>("Box Collider", entity, [this](auto& component, auto& entity) {
	});


	ImGui::SameLine();
	ImGui::PushItemWidth(-1);

	if (ImGui::Button("Add Component"))
		ImGui::OpenPopup("AddComponent");

	if (ImGui::BeginPopup("AddComponent"))
	{
		DisplayAddComponentEntry<MeshComponent>("Mesh");
		DisplayAddComponentEntry<MeshRendererComponent>("Mesh Renderer");
		DisplayAddComponentEntry<ScriptComponent>("Script");
		DisplayAddComponentEntry<CameraComponent>("Camera");
		DisplayAddComponentEntry<SpriteRendererComponent>("Sprite Renderer");
		DisplayAddComponentEntry<RigidBodyComponent>("Rigid Body");
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

template <>
void EntityPanel::InitComponent<Arcane::CameraComponent>()
{	
	m_Context.AddComponent<Arcane::CameraComponent>();
}


template <>
void EntityPanel::InitComponent<Arcane::SpriteRendererComponent>()
{
	// Generate Quad
	Arcane::SpriteRendererComponent spriteRenderer;
	spriteRenderer.color = {1.0f, 1.0f, 1.0f};
	spriteRenderer.quad = new Arcane::Quad();

	// Get default shader
	spriteRenderer.material = Arcane::Material::Create(Arcane::ShaderLibrary::GetShader("Sprite-Default"));

	m_Context.AddComponent<Arcane::SpriteRendererComponent>(spriteRenderer);
}

template <>
void EntityPanel::InitComponent<Arcane::RigidBodyComponent>()
{
	Arcane::RigidBodyComponent rigidBody;

	// Body Def 
	Kinetics::BodyDef bodyDef;
	bodyDef.mass = 1;

	Kinetics::DynamicBody* newBody = m_Context.GetScene()->AddDynamicBodyToPhysicsWorld(bodyDef);
	
	if (m_Context.HasComponent<Arcane::TransformComponent>()) {
		newBody->SetPosition({
			m_Context.GetComponent<Arcane::TransformComponent>().pos.x,
			m_Context.GetComponent<Arcane::TransformComponent>().pos.y,
			m_Context.GetComponent<Arcane::TransformComponent>().pos.z
		});
	}

	if (m_Context.HasComponent<Arcane::BoxColliderComponent>()) {
		newBody->SetShape(m_Context.GetComponent<Arcane::BoxColliderComponent>().shape);
	}

	rigidBody.body = newBody;
	rigidBody.gravityScale = 1;
	rigidBody.mass = bodyDef.mass;

	m_Context.AddComponent<Arcane::RigidBodyComponent>(rigidBody);
}

template<>
void EntityPanel::InitComponent<Arcane::BoxColliderComponent>()
{
	Arcane::BoxColliderComponent collider;

	collider.shape = new Kinetics::Shape({ Kinetics::ShapeType::BOX });

	// Need to set bouding box when creating a box collider 
	if (m_Context.HasComponent<Arcane::MeshComponent>()) {
		Arcane::BoundingBox& generatedBox = m_Context.GetComponent<Arcane::MeshComponent>().mesh->GetBoundingBox();
		
		Kinetics::Vec3 min = { generatedBox.bbMin.x, generatedBox.bbMin.y, generatedBox.bbMin.z };
		Kinetics::Vec3 max = { generatedBox.bbMax.x, generatedBox.bbMax.y, generatedBox.bbMax.z };

		collider.shape->SetBoundingBoxDimensions(min, max);
	}


	if (m_Context.HasComponent<Arcane::RigidBodyComponent>())
		m_Context.GetComponent<Arcane::RigidBodyComponent>().body->SetShape(collider.shape);

	m_Context.AddComponent<Arcane::BoxColliderComponent>(collider);
}