#include "SpriteEditor.h"
#include "PanelStructs.h"

SpriteEditor::SpriteEditor()
{

}

void SpriteEditor::OnImGuiRender()
{
	using namespace Arcane;

	const char* items[] = {"Single", "Multiple"};

	std::map<int, TextureImageAmountType> IndexToType = {
		{0, TextureImageAmountType::SINGLE},
		{1, TextureImageAmountType::MULTIPLE}
	};

	std::map<TextureImageAmountType,int> TypeToIndex = {
		{TextureImageAmountType::SINGLE, 0},
		{TextureImageAmountType::MULTIPLE,1}
	};

	static int item_current_idx = 0;

	ImGui::Begin("Sprite Editor");
	{
		if (m_Texture == nullptr) {
			ImGui::Text("Drag and Drop Texture");
		}
		else {
			TextureSpecs& textureSpecs = m_Texture->GetTextureSpecs();
			ImGui::InputInt("Width", &(int)textureSpecs.width);
			ImGui::InputInt("Height", &(int)textureSpecs.height);

			item_current_idx = TypeToIndex[textureSpecs.amountType];
			const char* combo_label = items[item_current_idx];

			if (ImGui::BeginCombo("Texture Type", combo_label))
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					const bool is_selected = (item_current_idx == n);
					
					if (ImGui::Selectable(items[n], is_selected))
						item_current_idx = n;

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			textureSpecs.amountType = IndexToType[item_current_idx];

			if (textureSpecs.amountType == TextureImageAmountType::MULTIPLE)
			{
			}

			Arcane::UI::Image(m_Texture);
		}

		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CURRENT_SELECTED_ASSET");

			if (payload != nullptr) {
				AssetInfo assetInfo = *static_cast<AssetInfo*>(payload->Data);
				Asset* asset = Arcane::Application::Get().GetAssetDatabase().GetAsset(assetInfo.id);

				if (asset != nullptr && asset->GetAssetType() == AssetType::TEXTURE)
				{
					Texture* texture = static_cast<Texture*>(asset);
					m_Texture = texture;
				}
			}
			ImGui::EndDragDropTarget();
		}
	}
	ImGui::End();
}
