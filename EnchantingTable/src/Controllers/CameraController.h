#pragma once

#include <Arcane.h>

class CameraController
{
public:
	CameraController() {};

	virtual void OnUpdate() {};

	void SetCamera(Arcane::Camera* camera) { m_Camera = camera; }
	Arcane::Camera* GetCamera() { return m_Camera; }
protected:
	Arcane::Camera* m_Camera;
};