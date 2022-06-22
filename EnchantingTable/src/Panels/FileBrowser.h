#pragma once
#include <Arcane.h>
#include <imgui.h>

class FileBrowserPanel
{
public:
	FileBrowserPanel();

	void OnUpdate();
private:
	Arcane::FileWatcher* m_Watcher;

};