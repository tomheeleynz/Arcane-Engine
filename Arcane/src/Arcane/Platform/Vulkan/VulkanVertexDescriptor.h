#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "Arcane/Renderer/VertexDescriptor.h"

namespace Arcane {

	class VulkanVertexDescriptor : public VertexDescriptor
	{
	public:
		VulkanVertexDescriptor(std::initializer_list<VertexType> vertexTypes);

		VkVertexInputBindingDescription GetBindingDescription() { return m_BindingDescription; }
		std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions() { return m_AttributeDescriptions; }

	private:
		VkVertexInputBindingDescription m_BindingDescription;
		std::vector<VkVertexInputAttributeDescription> m_AttributeDescriptions;
	};
}