#include "EntityPanel.h"
#include "PanelStructs.h"

#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

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

static void DrawVec3Control(std::string label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
{
	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());

	ImGui::NextColumn();
	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0.0f, 0.0f});

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight};

	// X Value
	if (ImGui::Button("X", buttonSize))
		values.x = resetValue;

	ImGui::SameLine();
	ImGui::DragFloat("##X", &values.x, 0.1f);
	ImGui::PopItemWidth();
	ImGui::SameLine();

	// Y Value
	if (ImGui::Button("Y", buttonSize))
		values.y = resetValue;

	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, 0.1f);
	ImGui::PopItemWidth();
	ImGui::SameLine();

	// Z Value
	if (ImGui::Button("Z", buttonSize))
		values.z = resetValue;

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &values.z, 0.1f);
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);
	ImGui::PopID();
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
		// ImGui::InputFloat3("Position", glm::value_ptr(component.pos));
		DrawVec3Control("Position", component.pos);

		// Rotation
		glm::vec3 rotation = glm::degrees(component.rotation);
		DrawVec3Control("Rotation", rotation);
		component.rotation = glm::radians(rotation);
	
		// Scale
		DrawVec3Control("Scale", component.scale);
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
					std::string change = std::any_cast<std::string>(val.value);

					ImGui::InputText(key.c_str(), &change);

					if (change != value) {
						component.updateProperties = true;
						component.script->SetPropertyValue(key, change);
					}
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

				if (type == "Vector2") {
					glm::vec2 change = std::any_cast<glm::vec2>(val.value);
					glm::vec2 value = std::any_cast<glm::vec2>(val.value);

					ImGui::InputFloat2(key.c_str(), glm::value_ptr(change));

					if (change.x != value.x || change.y != value.y)
					{
						component.updateProperties = true;
						component.script->SetPropertyValue(key, change);
					}
					
				}

				if (type == "Vector3") {
					glm::vec3 change = std::any_cast<glm::vec3>(val.value);
					glm::vec3 value = std::any_cast<glm::vec3>(val.value);

					DrawVec3Control(key, change);

					if (change.x != value.x || change.y != value.y || change.z != value.z)
					{
						component.updateProperties = true;
						component.script->SetPropertyValue(key, change);
					}
				}

				if (type == "Entity")
				{
					Arcane::Entity* change = nullptr;
					Arcane::Entity * value = nullptr;

					if (change == nullptr) {
						ImGui::InputText(key.c_str(), &std::string("Empty Entity"));

						if (ImGui::BeginDragDropTarget())
						{
							const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CURRENT_SELECTED_ENTITY");

							if (payload != nullptr) {
							}

							ImGui::EndDragDropTarget();
						}
					}
					else {
						std::string entityPropertyName = change->GetComponent<TagComponent>().tag;
						ImGui::InputText(key.c_str(), &entityPropertyName);

						if (ImGui::BeginDragDropTarget())
						{
							const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CURRENT_SELECTED_ENTITY");

							if (payload != nullptr) {
							}

							ImGui::EndDragDropTarget();
						}

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
		ImGui::ColorPicker3("Color", glm::value_ptr(entity.GetComponent<SpriteRendererComponent>().color));
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