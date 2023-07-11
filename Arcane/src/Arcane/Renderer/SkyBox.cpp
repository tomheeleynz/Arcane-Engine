#include "SkyBox.h"
#include "Arcane/Renderer/Renderer.h"

// Platforms
#include "Arcane/Platform/Vulkan/VulkanSkyBox.h"

namespace Arcane
{
	SkyBox* SkyBox::Create(std::vector<Texture*> faces)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::Vulkan: return new VulkanSkyBox(faces);
		case RendererAPIType::OpenGL: return nullptr;
		default:
			break;
		}
	}
}