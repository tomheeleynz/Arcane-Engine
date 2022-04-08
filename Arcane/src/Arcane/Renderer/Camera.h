#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Arcane
{	
	struct ViewData
	{
		glm::vec3 CameraPosition;
		glm::vec3 CameraLookDir;
		glm::vec3 CameraUpDir;
	};

	class Camera
	{
	public:
		Camera() = default;

		glm::mat4 GetView() { return m_View; }
		glm::mat4 GetProject() { return m_Projection; }
		glm::mat4 GetViewProjection() { return m_View * m_Projection; }

		ViewData& GetViewData() { return m_ViewData; }
		void SetViewData(ViewData& data) { m_ViewData = data; }
	protected:
		glm::mat4 m_View;
		glm::mat4 m_Projection;
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


		float GetAngle() { return m_Angle; }
		void SetAngle(float angle) { m_Angle = angle; }
	private:
		float m_Angle;
	};
}