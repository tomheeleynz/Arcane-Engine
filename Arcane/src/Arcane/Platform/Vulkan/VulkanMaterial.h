#pragma once

#include "Arcane/Renderer/Material.h"

namespace Arcane
{
	class VulkanMaterial: public Material
	{
	public:
		VulkanMaterial(Shader* shader);
	private:
		Shader* m_Shader;
	};
}