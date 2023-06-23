#include <imgui.h>
#include <backends/imgui_impl_vulkan.h>
#include "PlatformImGui.h"

// Vulkan Platform Stuff
#include "Arcane/Platform/Vulkan/VulkanFramebuffer.h"
#include "Arcane/Platform/Vulkan/VulkanTexture.h"

namespace Arcane::UI
{
	void Image(ImTextureID id, ImVec2 size)
	{
		ImGui::Image(id, size);
	}

	void Image(Texture* texture)
	{
		VulkanTexture* vulkanTexture = static_cast<VulkanTexture*>(texture);
		VkDescriptorImageInfo textureInfo = vulkanTexture->GetImageDescriptorInfo();

		ImGui::Image((ImTextureID)ImGui_ImplVulkan_AddTexture(textureInfo.sampler, textureInfo.imageView, textureInfo.imageLayout), ImVec2{ 128, 128 });
	}

	bool ImageButton(ImTextureID id, ImVec2 size)
	{
		return ImGui::ImageButton(id, size);
	}

	bool ImageButton(Texture* texture, ImVec2 size)
	{
		ImTextureID id = AddTexture(texture);
		return ImGui::ImageButton(id, size);
	}

	ImTextureID AddTexture(Texture* texture)
	{
		VulkanTexture* vulkanTexture = static_cast<VulkanTexture*>(texture);
		VkDescriptorImageInfo textureInfo = vulkanTexture->GetImageDescriptorInfo();
		return ImGui_ImplVulkan_AddTexture(textureInfo.sampler, textureInfo.imageView, textureInfo.imageLayout);
	}

	ImTextureID AddTexture(Framebuffer* frameBuffer)
	{
		VulkanFramebuffer* vulkanFramebuffer = static_cast<VulkanFramebuffer*>(frameBuffer);
		VkDescriptorImageInfo imageDescriptor = static_cast<VulkanColorAttachment*>(frameBuffer->GetColorAttachment(0))->imageInfo;
		return ImGui_ImplVulkan_AddTexture(imageDescriptor.sampler, imageDescriptor.imageView, imageDescriptor.imageLayout);
	}
}