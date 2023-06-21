#pragma once

#include <Arcane.h>

#include "CameraController.h"

class PerspectiveController : public CameraController
{
public:
	PerspectiveController();

	virtual void OnUpdate();
private:
	glm::vec2 m_LastMousePos;
	bool m_FirstUpdate = true;
};