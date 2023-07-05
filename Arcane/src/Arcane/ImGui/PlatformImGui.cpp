#include <imgui.h>
#include <backends/imgui_impl_vulkan.h>
#include "PlatformImGui.h"
#include "Arcane/Renderer/Renderer.h"

// Vulkan Platform Stuff
#include "Arcane/Platform/Vulkan/VulkanFramebuffer.h"
#include "Arcane/Platform/Vulkan/VulkanTexture.h"

// Opengl Platform Stuff
#include "Arcane/Platform/OpenGL/OpenGLTexture.h"
#include "Arcane/Platform/OpenGL/OpenGLFramebuffer.h"

namespace Arcane::UI
{
	void Image(ImTextureID id, ImVec2 size)
	{
		ImGui::Image(id, size);
	}

	void Image(Texture* texture)
	{
		if (RendererAPI::Current() == RendererAPIType::Vulkan) {
			VulkanTexture* vulkanTexture = static_cast<VulkanTexture*>(texture);
			VkDescriptorImageInfo textureInfo = vulkanTexture->GetImageDescriptorInfo();
			ImGui::Image((ImTextureID)ImGui_ImplVulkan_AddTexture(textureInfo.sampler, textureInfo.imageView, textureInfo.imageLayout), ImVec2{ 128, 128 });
		}
		else if (RendererAPI::Current() == RendererAPIType::OpenGL) {
			OpenGLTexture* openglTexture = static_cast<OpenGLTexture*>(texture);
			ImGui::Image((ImTextureID)openglTexture->GetTextureID(), ImVec2{ 128, 128 });
		}
	}

	bool ImageButton(ImTextureID id, ImVec2 size)
	{
		return ImGui::ImageButton(id, size);
	}

	bool ImageButton(Texture* texture, ImVec2 size)
	{
		if (RendererAPI::Current() == RendererAPIType::Vulkan) {
			VulkanTexture* vulkanTexture = static_cast<VulkanTexture*>(texture);
			VkDescriptorImageInfo textureInfo = vulkanTexture->GetImageDescriptorInfo();
			return ImGui::ImageButton((ImTextureID)ImGui_ImplVulkan_AddTexture(textureInfo.sampler, textureInfo.imageView, textureInfo.imageLayout), size);
		}
		else if (RendererAPI::Current() == RendererAPIType::OpenGL) {
			OpenGLTexture* openglTexture = static_cast<OpenGLTexture*>(texture);
			return ImGui::ImageButton((ImTextureID)openglTexture->GetTextureID(), size);
		}

		return false;
	}

	ImTextureID AddTexture(Texture* texture)
	{
		if (RendererAPI::Current() == RendererAPIType::Vulkan) {
			VulkanTexture* vulkanTexture = static_cast<VulkanTexture*>(texture);
			VkDescriptorImageInfo textureInfo = vulkanTexture->GetImageDescriptorInfo();
			return ImGui_ImplVulkan_AddTexture(textureInfo.sampler, textureInfo.imageView, textureInfo.imageLayout);
		}
		else if (RendererAPI::Current() == RendererAPIType::OpenGL) {
			OpenGLTexture* openglTexture = static_cast<OpenGLTexture*>(texture);
			return (ImTextureID)openglTexture->GetTextureID();
		}
	}

	ImTextureID AddTexture(Framebuffer* frameBuffer)
	{
		if (RendererAPI::Current() == RendererAPIType::Vulkan) {
			VulkanFramebuffer* vulkanFramebuffer = static_cast<VulkanFramebuffer*>(frameBuffer);
			VkDescriptorImageInfo textureInfo = vulkanFramebuffer->GetDescriptor();
			return ImGui_ImplVulkan_AddTexture(textureInfo.sampler, textureInfo.imageView, textureInfo.imageLayout);
		}
		else if (RendererAPI::Current() == RendererAPIType::OpenGL) {
			OpenGLFramebuffer* openglFramebuffer = static_cast<OpenGLFramebuffer*>(frameBuffer);
			return (ImTextureID)openglFramebuffer->GetFramebufferID();
		}
	}
}