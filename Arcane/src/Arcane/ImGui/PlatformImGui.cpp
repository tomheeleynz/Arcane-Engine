#include  <imgui.h>
#include <backends/imgui_impl_vulkan.h>
#include "PlatformImGui.h"

// Vulkan Platform Stuff
#include "Arcane/Platform/Vulkan/VulkanFramebuffer.h"
#include "Arcane/Platform/Vulkan/VulkanTexture.h"

namespace Arcane::UI 
{
	void Image(Framebuffer* framebuffer) 
	{
		VulkanFramebuffer* vulkanFramebuffer = static_cast<VulkanFramebuffer*>(framebuffer);
		VkDescriptorImageInfo imageDescriptor = vulkanFramebuffer->GetDescriptor();
		
		ImGui::Image((ImTextureID)ImGui_ImplVulkan_AddTexture(imageDescriptor.sampler, imageDescriptor.imageView, imageDescriptor.imageLayout), ImVec2{1, 1});
	}

	void Image(Texture* texture)
	{
		VulkanTexture* vulkanTexture = static_cast<VulkanTexture*>(texture);
		VkDescriptorImageInfo textureInfo = vulkanTexture->GetImageDescriptorInfo();

		ImGui::Image((ImTextureID)ImGui_ImplVulkan_AddTexture(textureInfo.sampler, textureInfo.imageView, textureInfo.imageLayout), ImVec2{ 512, 512 });
	}
}