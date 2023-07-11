#pragma once

#include <Arcane.h>

class EnvironmentPanel
{
public:
	EnvironmentPanel();


	void OnImGuiRender();
private:
	Arcane::SkyBox* m_Skybox;
	std::vector<Arcane::Texture*> m_Faces;
};