#include "FileBrowser.h"

FileBrowserPanel::FileBrowserPanel()
{
	m_Watcher = new Arcane::FileWatcher("./src/Assets", std::chrono::milliseconds(5000));
	m_Icons["Folder"] = Arcane::Texture::Create("./src/Icons/folder_icon.png");
}

void FileBrowserPanel::OnUpdate()
{
	m_Watcher->Update();
	
	ImGui::Begin("File Browser");
	{
		if (ImGui::Button("Back")) {
			m_Watcher->GoToParentDirectory();
		}
		
		for (auto& path : m_Watcher->GetPaths())
		{
			if (!path.second.isDirectory) {
				ImGui::Text(path.first.c_str());
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
					ImGui::SetDragDropPayload("CURRENT_SELECTED_ASSET", &path.second.assetID, sizeof(int));
					ImGui::EndDragDropSource();
				}
			}
			else {

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.3f));

				if (Arcane::UI::ImageButton(m_Icons["Folder"], {50, 50})) {
					m_Watcher->SetDirectory(path.second.relativePath);
				}

				ImGui::PopStyleColor(2);
				ImGui::Text(path.second.relativePath.stem().string().c_str());
			}
		}
	}
	ImGui::End();
}

std::string FileBrowserPanel::GetIconType(std::string extension)
{
	return std::string();
}