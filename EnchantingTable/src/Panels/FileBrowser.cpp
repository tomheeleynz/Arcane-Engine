#include "FileBrowser.h"

FileBrowserPanel::FileBrowserPanel()
{
	m_Watcher = new Arcane::FileWatcher("./", std::chrono::milliseconds(5000));
}

void FileBrowserPanel::OnUpdate()
{
	ImGui::Begin("File Browser");
	{

	}
	ImGui::End();
}