#pragma once

#include <Arcane.h>
#include <imgui.h>
#include <iostream>

class MaterialViewerPanel
{
public:
	MaterialViewerPanel();

	void OnUpdate();

	void SetMaterial(Arcane::Material* material);
	Arcane::Material* GetMaterial() { return m_Material; }

	void DisplayMaterialVariable(Arcane::ShaderMember& member, Arcane::ShaderBindingType type, uint32_t binding);
private:
	Arcane::Material* m_Material = nullptr;
};