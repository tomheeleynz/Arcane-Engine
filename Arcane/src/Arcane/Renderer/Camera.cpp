#include "Camera.h"

namespace Arcane
{
	//////////////////////////////////////////////////////////
	//// Orthographic Camera
	//////////////////////////////////////////////////////////
	OrthoCamera::OrthoCamera(uint32_t width, uint32_t height)
	{
		uint32_t halfWidth = width / 2;
		uint32_t halfHeight = height / 2;
		
		m_Projection = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight);
		
		m_View = glm::lookAt(
			m_ViewData.CameraPosition,
			m_ViewData.CameraPosition + m_ViewData.CameraLookDir,
			m_ViewData.CameraUpDir
		);
	}

	//////////////////////////////////////////////////////////
	//// Perspective Camera
	//////////////////////////////////////////////////////////
	PerspectiveCamera::PerspectiveCamera(uint32_t width, uint32_t height, float angle)
	{
		m_Angle = angle;

		m_Projection = glm::perspective(glm::radians(angle), (float)width/(float)height, 0.1f, 10000.0f);

		m_View = glm::lookAt(
			m_ViewData.CameraPosition,
			m_ViewData.CameraPosition + m_ViewData.CameraLookDir,
			m_ViewData.CameraUpDir
		);
	}
}