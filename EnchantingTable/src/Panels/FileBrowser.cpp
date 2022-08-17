#include "FileBrowser.h"

FileBrowserPanel::FileBrowserPanel()
{
	m_Watcher = new Arcane::FileWatcher("./src/Assets/Models", std::chrono::milliseconds(5000));
}

void FileBrowserPanel::OnUpdate()
{
	m_Watcher->Update();
	
	ImGui::Begin("File Browser");
	{
		for (auto& path : m_Watcher->GetPaths())
		{
			ImGui::Text(path.first.c_str());

			if (!path.second.isDirectory) {
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
					ImGui::SetDragDropPayload("CURRENT_SELECTED_ASSET", &path.second.assetID, sizeof(int));
					ImGui::EndDragDropSource();
				}
			}

		}
	}
	ImGui::End();
}