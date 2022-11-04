#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace Arcane
{	
	struct ViewData
	{
		glm::vec3 CameraPosition = {0.0f, 0.0f, 3.0f};
		glm::vec3 CameraLookDir = {0.0f, 0.0f, 0.0f};
		glm::vec3 CameraUpDir = {0.0f, 1.0f, 0.0f};
	};

	class Camera
	{
	public:
		Camera() = default;

		glm::mat4 GetView() { return m_View; }
		glm::mat4 GetProject() { return m_Projection; }
		glm::mat4 GetViewProjection() { return m_View * m_Projection; }

		glm::vec3 GetViewDir() { return -glm::transpose(m_View)[2]; }
		glm::vec3 GetRightVector() { return glm::transpose(m_View)[0]; }

		glm::vec3 GetPosition() { return m_ViewData.CameraPosition; }

		glm::vec2 GetSize() { return m_Size; }
		void SetSize(uint32_t width, uint32_t height) { m_Size = {width, height}; }

		ViewData& GetViewData() { return m_ViewData; }
		void SetViewData(ViewData& data);
	protected:
		glm::mat4 m_View;
		glm::mat4 m_Projection;
		glm::vec2 m_Size;
		ViewData m_ViewData;
	};


	class OrthoCamera : public Camera
	{
	public:
		OrthoCamera(uint32_t width, uint32_t height);

	private:

	};

	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(uint32_t width, uint32_t height, float angle);

		void OnResize(uint32_t width, uint32_t height);
		float GetAngle() { return m_Angle; }
		void SetAngle(float angle) { m_Angle = angle; }
	private:
		float m_Angle;
	};
}