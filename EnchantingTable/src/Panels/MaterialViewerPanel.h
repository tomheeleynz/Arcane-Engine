#pragma once

#include <Arcane.h>
#include <imgui.h>

class MaterialViewerPanel
{
public:
	MaterialViewerPanel();

	void OnUpdate();

	void SetMaterial(Arcane::Material* material);
	Arcane::Material* GetMaterial() { return m_Material; }
private:
	Arcane::Material* m_Material = nullptr;
};