#include "MaterialViewerPanel.h"
#include "PanelStructs.h"

#include <glm/gtc/type_ptr.hpp>

MaterialViewerPanel::MaterialViewerPanel()
{

}

void MaterialViewerPanel::OnUpdate()
{
	using namespace Arcane;
	if (m_Material == nullptr) return;
	
	ImGui::Begin("Material Viewer");

	ImGui::Text("Shader");

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CURRENT_SELECTED_ASSET");

		if (payload != nullptr) {
			AssetInfo assetInfo = *static_cast<AssetInfo*>(payload->Data);
			Asset* asset = Arcane::Application::Get().GetAssetDatabase().GetAsset(assetInfo.id);

			if (asset != nullptr && asset->GetAssetType() == AssetType::SHADER)
			{
				Shader* shader = static_cast<Shader*>(asset);
				m_Material->SetShader(shader);
			}
		}
		ImGui::EndDragDropTarget();
	}

	if (m_Material->GetShader() != nullptr) {
		ShaderSet& set = m_Material->GetMaterialSet();

		for (int i = 0; i < set.Bindings.size(); i++) {
			ShaderBinding& binding = set.Bindings[i];

			for (int j = 0; j < binding.Members.size(); j++) {
				ShaderMember& member = binding.Members[j];
				DisplayMaterialVariable(member, binding.Type, binding.Binding);
			}
		}
	}
	
	if (ImGui::Button("Save Material"))
	{
		std::string filename = Arcane::FileDialog::SaveFile();

		if (!filename.empty()) {
			Arcane::MaterialSerializer serializer(m_Material);
			serializer.Serialize(filename);
		}
	}

	ImGui::End();
}

void MaterialViewerPanel::SetMaterial(Arcane::Material* material)
{
	m_Material = material;
}

void MaterialViewerPanel::DisplayMaterialVariable(Arcane::ShaderMember& member, Arcane::ShaderBindingType type, uint32_t binding)
{
	using namespace Arcane;
	if (member.Members.size() != 0) {
		std::string variableName = "--" + member.Name;
		ImGui::Text(variableName.c_str());

		for (int i = 0; i < member.Members.size(); i++) {
			DisplayMaterialVariable(member.Members[i], type, binding);
		}
	}
	else {
		if (type == Arcane::ShaderBindingType::SAMPLER) {
			Arcane::Texture* texture = m_Material->GetTexture(binding);

			if (texture == nullptr) 
				ImGui::Text(member.Name.c_str());
			else
				Arcane::UI::Image(texture);

			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CURRENT_SELECTED_ASSET");

				if (payload != nullptr) {
					AssetInfo assetInfo = *static_cast<AssetInfo*>(payload->Data);
					Asset* asset = Arcane::Application::Get().GetAssetDatabase().GetAsset(assetInfo.id);

					if (asset != nullptr && asset->GetAssetType() == AssetType::TEXTURE)
					{
						Texture* texture = static_cast<Texture*>(asset);
						m_Material->WriteTexture(binding, texture);
					}
				}
				ImGui::EndDragDropTarget();
			}

			if (texture != nullptr) {
				ImGui::Text(member.Name.c_str());
			}
		}
		else {
			if (member.type == ShaderMemberType::Vec4)
			{
				glm::vec3 value = m_Material->GetVec3(binding, member.Offset);
				ImGui::InputFloat3(member.Name.c_str(), glm::value_ptr(value));
				m_Material->WriteVec3(binding, member.Offset, value);
			}
			else if (member.type == ShaderMemberType::Vec2)
			{
				glm::vec2 test = { 0.0f, 0.0f };
				ImGui::InputFloat2(member.Name.c_str(), glm::value_ptr(test));
			}
		}
		return;
	}
}
