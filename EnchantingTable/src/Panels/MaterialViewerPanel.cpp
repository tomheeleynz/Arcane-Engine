#include "MaterialViewerPanel.h"
#include "PanelStructs.h"

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
		std::vector<ShaderSet> sets = m_Material->GetShaderSets();
			
		// This will display the material set
		for (int i = 0; i < sets.size(); i++) {
			if (sets[i].SetNumber == 2) {
				
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
