#pragma once

#include <Arcane.h>

class PerspectiveController
{
public:
	PerspectiveController();

	void OnUpdate();

	void SetCamera(Arcane::Camera* camera);
	Arcane::Camera* GetCamera();
private:
	Arcane::Camera* m_Camera;
	glm::vec2 m_LastMousePos;
	bool m_FirstUpdate = true;
};