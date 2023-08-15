#pragma once

#include <vulkan/vulkan.h>
#include "Arcane/Renderer/Texture.h"


namespace Arcane 
{
	class VulkanTexture : public Texture
	{
	public:
		VulkanTexture(std::string fileName);
		VulkanTexture(float r, float g, float b, float a);
		VulkanTexture(char* data, uint32_t size, uint32_t width, uint32_t height);

		virtual void UpdateTexture(float r, float g, float b, float a) override;
		virtual void UpdateTexture(std::string filename) override;
		virtual void UpdateTexture(Texture* texture) override;

		TextureImageDataType GetTextureDataType() override;
		void SetTextureDataType(TextureImageDataType type) override;

		glm::vec4 GetRGBAValue() override;
		void SetRGBAValue(glm::vec4 value) override;

		VkDescriptorImageInfo GetImageDescriptorInfo() { return m_ImageInfo; }

		TextureSpecs& GetTextureSpecs() { return m_Specs; }

		void SetTextureAmountType(TextureImageAmountType type) { m_Specs.amountType = type; }

		TextureRuntimeData GetRuntimeData();

		virtual void PackAsset(std::ofstream& o) override;
		virtual std::pair<uint64_t, Asset*> UnpackAsset(std::ofstream& o) override;
	private:
		VkCommandBuffer BeginSingleTimeCommands();
		void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

		void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

		VkImageView CreateImageView(VkImage image, VkFormat format);
		void CreateTextureImageView();
		void CreateTextureSampler();
	private:
		VkImage m_TextureImage;
		VkDeviceMemory m_TextureImageMemory;
		VkImageView m_TextureImageView;
		VkSampler m_TextureSampler;
		VkDescriptorImageInfo m_ImageInfo;
		TextureImageDataType m_TextureDataType;
		glm::vec4 m_RGBAValue;
		TextureSpecs m_Specs;
	};
}