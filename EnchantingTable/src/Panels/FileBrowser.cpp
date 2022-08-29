#include "FileBrowser.h"

FileBrowserPanel::FileBrowserPanel()
{
	m_Watcher = new Arcane::FileWatcher("./src/Assets", std::chrono::milliseconds(5000));
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
				if (ImGui::Button(path.first.c_str())) {
					m_Watcher->SetDirectory(path.second.relativePath);
				}
			}
		}
	}
	ImGui::End();
}