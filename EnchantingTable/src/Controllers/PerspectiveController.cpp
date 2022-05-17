#include "PerspectiveController.h"


PerspectiveController::PerspectiveController()
{

}

void PerspectiveController::OnUpdate()
{
	if (Arcane::InputManager::GetMouseKeyPressed(0)) {
		glm::vec4 position = glm::vec4(m_Camera->GetViewData().CameraPosition, 1);
		glm::vec4 pivot = glm::vec4(m_Camera->GetViewData().CameraLookDir, 1);

		float deltaX = ((2 * 3.14159) / m_Camera->GetSize().x);
		float deltaY = (3.14159 / m_Camera->GetSize().y);

		glm::vec2 MousePos = { Arcane::InputManager::GetMouseCoords().first,Arcane::InputManager::GetMouseCoords().second};
		
		if (m_FirstUpdate) {
			m_LastMousePos = MousePos;
			m_FirstUpdate = false;
		}

		float xAngle = (m_LastMousePos.x - MousePos.x) * deltaX;
		float yAngle = (m_LastMousePos.y - MousePos.y) * deltaY;

		// Rotation around x axis
		glm::mat4 rotationX = glm::mat4(1.0f);
		rotationX = glm::rotate(rotationX, xAngle, m_Camera->GetViewData().CameraUpDir);
		position = (rotationX * (position - pivot)) + pivot;

		// rotation around y axis
		glm::mat4 rotationY = glm::mat4(1.0f);
		rotationY = glm::rotate(rotationY, yAngle, m_Camera->GetRightVector());
		glm::vec3 finalPosition = (rotationY * (position - pivot)) + pivot;

		// Set Camera View
		Arcane::ViewData newData;
		newData.CameraPosition = finalPosition;
		newData.CameraLookDir = m_Camera->GetViewData().CameraLookDir;
		newData.CameraUpDir = m_Camera->GetViewData().CameraUpDir;
		m_Camera->SetViewData(newData);

		m_LastMousePos = MousePos;
	}
	else if (Arcane::InputManager::GetKeyReleased(0)) {
		m_FirstUpdate = true;
	}

	// -- Camera Distance
	glm::vec3 zoomDir = m_Camera->GetViewData().CameraLookDir - m_Camera->GetViewData().CameraPosition;
	glm::vec3 newPos = m_Camera->GetViewData().CameraPosition + (zoomDir * (Arcane::InputManager::GetScrollOffsets().y * 0.1f));

	Arcane::ViewData newData;
	newData.CameraPosition = newPos;
	newData.CameraLookDir = m_Camera->GetViewData().CameraLookDir;
	newData.CameraUpDir = m_Camera->GetViewData().CameraUpDir;
	m_Camera->SetViewData(newData);
}

void PerspectiveController::SetCamera(Arcane::Camera* camera)
{
	m_Camera = camera;
}

Arcane::Camera* PerspectiveController::GetCamera()
{
	return m_Camera;
}
