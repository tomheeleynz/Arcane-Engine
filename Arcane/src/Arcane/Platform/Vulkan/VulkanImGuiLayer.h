#pragma once

#include <imgui.h>
#include "ImGui/ImGuiLayer.h"
#include "backends/imgui_impl_vulkan.h"
#include "backends/imgui_impl_glfw.h"

namespace Arcane {
	class VulkanImGuiLayer : public ImGuiLayer
	{
	public:
		VulkanImGuiLayer();

		void Begin() override;
		void End() override;

		void OnAttach() override;
	private:
	};
}