#include "SkyBox.h"
#include "Arcane/Renderer/Renderer.h"

// Platforms
#include "Arcane/Platform/Vulkan/VulkanSkyBox.h"

namespace Arcane
{
	SkyBox* SkyBox::Create(uint64_t assetID)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::Vulkan: return new VulkanSkyBox(assetID);
		case RendererAPIType::OpenGL: return nullptr;
		default:
			break;
		}
	}
}