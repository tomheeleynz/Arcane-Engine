#include "Camera.h"

namespace Arcane
{
	//////////////////////////////////////////////////////////
	//// Base Camera
	//////////////////////////////////////////////////////////
	void Camera::SetViewData(ViewData& data)
	{
		m_ViewData = data;
		m_View = glm::lookAt(data.CameraPosition, data.CameraLookDir, data.CameraUpDir);
	}

	//////////////////////////////////////////////////////////
	//// Orthographic Camera
	//////////////////////////////////////////////////////////
	OrthoCamera::OrthoCamera(uint32_t width, uint32_t height)
	{
		uint32_t halfWidth = width / 2;
		uint32_t halfHeight = height / 2;
		
		m_Projection = glm::ortho(-(float)halfWidth, (float)halfWidth, -(float)halfHeight, (float)halfHeight, -1.0f, 1.0f);
		
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_ViewData.CameraPosition) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_View = glm::inverse(transform);

		m_Height = height;
		m_Width = width;
	}

	void OrthoCamera::OnResize(uint32_t width, uint32_t height) 
	{
		uint32_t halfWidth = width / 2;
		uint32_t halfHeight = height / 2;

		m_Projection = glm::ortho(-(float)halfWidth, (float)halfWidth, -(float)halfHeight, (float)halfHeight, -1.0f, 1.0f);
		
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_ViewData.CameraPosition) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_View = glm::inverse(transform);
	}

	void OrthoCamera::SetNearPlane(float nearPlane)
	{
		uint32_t halfWidth = m_Width / 2;
		uint32_t halfHeight = m_Height / 2;

		m_NearPlane = nearPlane;

		m_Projection = glm::ortho(-(float)halfWidth, (float)halfWidth, -(float)halfHeight, (float)halfHeight, nearPlane, m_FarPlane);
	}

	void OrthoCamera::SetFarPlane(float farPlane)
	{
		uint32_t halfWidth = m_Width / 2;
		uint32_t halfHeight = m_Height / 2;
		m_FarPlane = farPlane;

		m_Projection = glm::ortho(-(float)halfWidth, (float)halfWidth, -(float)halfHeight, (float)halfHeight, m_NearPlane, farPlane);
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
			m_ViewData.CameraLookDir,
			m_ViewData.CameraUpDir
		);
	}

	void PerspectiveCamera::OnResize(uint32_t width, uint32_t height)
	{
		SetSize(width, height);
		m_Projection = glm::perspective(glm::radians(m_Angle), (float)width/(float)height, 0.1f, 10000.0f);
	}
}