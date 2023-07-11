#pragma once

#include "Arcane/Renderer/SkyBox.h"
#include "Arcane/Renderer/Buffer.h"

namespace Arcane
{
	class VulkanSkyBox : public SkyBox
	{
	public:
		VulkanSkyBox(std::vector<Texture*> textures);

	private:
		VertexBuffer* m_SkyboxVertexBuffer;
	};
}