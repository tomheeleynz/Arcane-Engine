#pragma once

#include <imgui.h>
#include <string>
#include <glad/glad.h>

#include "Arcane/Core/Application.h"
#include "Arcane/ImGui/ImGuiLayer.h"

namespace Arcane
{
	class OpenGLImguiLayer : public ImGuiLayer
	{
	public:
		OpenGLImguiLayer();

		virtual void Begin() override;
		virtual void End() override;

		virtual void OnAttach() override;
	private:
	
	};
}