#include "VulkanVertexDescriptor.h"

namespace Arcane {

	VulkanVertexDescriptor::VulkanVertexDescriptor(std::initializer_list<VertexType> vertexTypes)
	{
		// Calculating Stride
		uint32_t stride = 0;
		for (auto element : vertexTypes)
		{
			switch (element)
			{
			case VertexType::float1:
			{
				stride += sizeof(float);
				break;
			}
			case VertexType::float2:
			{
				stride += sizeof(float) * 2;
				break;
			}
			case VertexType::float3:
			{
				stride += sizeof(float) * 3;
				break;
			}
			case VertexType::float4:
			{
				stride += sizeof(float) * 4;
				break;
			}
			default:
				break;
			}
		}

		m_BindingDescription.binding = 0;
		m_BindingDescription.stride = stride;
		m_BindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		// Create Attribute Description
		int location = 0;
		int offset = 0;
		for (auto element : vertexTypes)
		{
			switch (element)
			{
			case VertexType::float1:
			{
				VkVertexInputAttributeDescription attribDesc;
				attribDesc.binding = 0;
				attribDesc.location = location;
				attribDesc.format = VK_FORMAT_R32_SFLOAT;
				attribDesc.offset = offset;

				location++;
				offset += sizeof(float);
				m_AttributeDescriptions.push_back(attribDesc);
				break;
			}
			case VertexType::float2:
			{
				VkVertexInputAttributeDescription attribDesc;
				attribDesc.binding = 0;
				attribDesc.location = location;
				attribDesc.format = VK_FORMAT_R32G32_SFLOAT;
				attribDesc.offset = offset;

				location++;
				offset += sizeof(float) * 2;
				m_AttributeDescriptions.push_back(attribDesc);
				break;
			}
			case VertexType::float3:
			{
				VkVertexInputAttributeDescription attribDesc;
				attribDesc.binding = 0;
				attribDesc.location = location;
				attribDesc.format = VK_FORMAT_R32G32B32_SFLOAT;
				attribDesc.offset = offset;

				location++;
				offset += sizeof(float) * 3;
				m_AttributeDescriptions.push_back(attribDesc);
				break;
			}
			case VertexType::float4:
			{
				VkVertexInputAttributeDescription attribDesc;
				attribDesc.binding = 0;
				attribDesc.location = location;
				attribDesc.format = VK_FORMAT_R32G32B32A32_SFLOAT;
				attribDesc.offset = offset;

				location++;
				offset += sizeof(float) * 4;
				m_AttributeDescriptions.push_back(attribDesc);
				break;
			}
			default:
				break;
			}
		}
	}
}