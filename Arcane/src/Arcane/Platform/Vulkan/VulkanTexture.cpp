#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "VulkanContext.h"
#include "VulkanTexture.h"
#include "Arcane/Core/Application.h"

namespace Arcane
{
	static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		Application& app = Application::Get();
		VkDevice logicalDevice = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetDevice().GetLogicalDevice();
		VkPhysicalDevice physicalDevice = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetPhysicalDevice().GetPhysicalDevice();

		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		return -1;
	}

	VulkanTexture::VulkanTexture(std::string fileName)
	{
		Application& app = Application::Get();
		VkDevice logicalDevice = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetDevice().GetLogicalDevice();
		
		int width, height, channels = 0;
		stbi_uc* pixels = stbi_load(fileName.c_str(), &width, &height, &channels, STBI_rgb_alpha);
		VkDeviceSize imageSize = width * height * 4;

		if (pixels) {
			VkBuffer stagingBuffer;
			VkDeviceMemory stagingBufferMemory;

			VkBufferCreateInfo bufferInfo{};
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferInfo.size = imageSize;
			bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			if (vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &stagingBuffer) != VK_SUCCESS) {
				printf("Texture Staging Buffer Not Created\n");
			}
			else {
				printf("Texture Buffer Created\n");
			}

			VkMemoryRequirements stagingMemRequirements;
			vkGetBufferMemoryRequirements(logicalDevice, stagingBuffer, &stagingMemRequirements);

			VkMemoryAllocateInfo stagingAllocInfo{};
			stagingAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			stagingAllocInfo.allocationSize = stagingMemRequirements.size;
			stagingAllocInfo.memoryTypeIndex = FindMemoryType(stagingMemRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			if (vkAllocateMemory(logicalDevice, &stagingAllocInfo, nullptr, &stagingBufferMemory) != VK_SUCCESS) {
				printf("Failed to allocated vertex buffer memory\n");
			}
			else {
				printf("Allocated vertex buffer memory\n");
			}

			void* data;
			vkMapMemory(logicalDevice, stagingBufferMemory, 0, imageSize, 0, &data);
			memcpy(data, pixels, static_cast<size_t>(imageSize));
			vkUnmapMemory(logicalDevice, stagingBufferMemory);

			// Creating Image Info
			VkImageCreateInfo imageInfo{};
			imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			imageInfo.imageType = VK_IMAGE_TYPE_2D;
			imageInfo.extent.width = static_cast<uint32_t>(width);
			imageInfo.extent.height = static_cast<uint32_t>(height);
			imageInfo.extent.depth = 1;
			imageInfo.mipLevels = 1;
			imageInfo.arrayLayers = 1;
			imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
			imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
			imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
			imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
			imageInfo.flags = 0;

			if (vkCreateImage(logicalDevice, &imageInfo, nullptr, &m_TextureImage) != VK_SUCCESS) {
				printf("Texture Image Not Created\n");
			}
			else {
				printf("Texture Image Created\n");
			}

			VkMemoryRequirements memRequirements;
			vkGetImageMemoryRequirements(logicalDevice, m_TextureImage, &memRequirements);

			VkMemoryAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

			if (vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &m_TextureImageMemory) != VK_SUCCESS) {
				printf("Texture Image Memory Not Allocated");
			}

			vkBindImageMemory(logicalDevice, m_TextureImage, m_TextureImageMemory, 0);
			vkBindBufferMemory(logicalDevice, stagingBuffer, stagingBufferMemory, 0);

			TransitionImageLayout(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
			
			CopyBufferToImage(stagingBuffer, m_TextureImage, static_cast<uint32_t>(width), static_cast<uint32_t>(height));
			
			TransitionImageLayout(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

			// Create Texture Image View
			CreateTextureImageView();

			// Create Texture Sampler
			CreateTextureSampler();

			// Set Up image Descriptor Info
			m_ImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			m_ImageInfo.imageView = m_TextureImageView;
			m_ImageInfo.sampler = m_TextureSampler;
		}
	}

	VulkanTexture::VulkanTexture(float r, float g, float b, float a)
	{
		Application& app = Application::Get();
		VkDevice logicalDevice = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetDevice().GetLogicalDevice();
		
		int width = 256;
		int height = 256;

		uint8_t* colors = new uint8_t[(width * height) * 4];
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		int i = 0;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				colors[i] = 255;
				colors[i + 1] = 0;
				colors[i + 2] = 0;
				colors[i + 3] = 0;
				i += 4;
			}
		}


		VkDeviceSize imageSize = width * height * 4;
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = imageSize;
		bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &stagingBuffer) != VK_SUCCESS) {
			printf("Texture Staging Buffer Not Created\n");
		}
		else {
			printf("Texture Buffer Created\n");
		}

		VkMemoryRequirements stagingMemRequirements;
		vkGetBufferMemoryRequirements(logicalDevice, stagingBuffer, &stagingMemRequirements);

		VkMemoryAllocateInfo stagingAllocInfo{};
		stagingAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		stagingAllocInfo.allocationSize = stagingMemRequirements.size;
		stagingAllocInfo.memoryTypeIndex = FindMemoryType(stagingMemRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		if (vkAllocateMemory(logicalDevice, &stagingAllocInfo, nullptr, &stagingBufferMemory) != VK_SUCCESS) {
			printf("Failed to allocated vertex buffer memory\n");
		}
		else {
			printf("Allocated vertex buffer memory\n");
		}

		void* data;
		vkMapMemory(logicalDevice, stagingBufferMemory, 0, imageSize, 0, &data);
		memcpy(data, (void*)&colors[0], static_cast<size_t>(imageSize));
		vkUnmapMemory(logicalDevice, stagingBufferMemory);

		// Creating Image Info
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = static_cast<uint32_t>(width);
		imageInfo.extent.height = static_cast<uint32_t>(height);
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.flags = 0;

		if (vkCreateImage(logicalDevice, &imageInfo, nullptr, &m_TextureImage) != VK_SUCCESS) {
			printf("Texture Image Not Created\n");
		}
		else {
			printf("Texture Image Created\n");
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(logicalDevice, m_TextureImage, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		if (vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &m_TextureImageMemory) != VK_SUCCESS) {
			printf("Texture Image Memory Not Allocated");
		}

		vkBindImageMemory(logicalDevice, m_TextureImage, m_TextureImageMemory, 0);
		vkBindBufferMemory(logicalDevice, stagingBuffer, stagingBufferMemory, 0);

		TransitionImageLayout(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		CopyBufferToImage(stagingBuffer, m_TextureImage, static_cast<uint32_t>(width), static_cast<uint32_t>(height));

		TransitionImageLayout(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		// Create Texture Image View
		CreateTextureImageView();

		// Create Texture Sampler
		CreateTextureSampler();

		// Set Up image Descriptor Info
		m_ImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		m_ImageInfo.imageView = m_TextureImageView;
		m_ImageInfo.sampler = m_TextureSampler;
	}

	void VulkanTexture::UpdateTexture(float r, float g, float b, float a)
	{
		Application& app = Application::Get();
		VkDevice logicalDevice = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetDevice().GetLogicalDevice();

		int width = 256;
		int height = 256;

		uint8_t* colors = new uint8_t[(width * height) * 4];
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		int i = 0;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < height; x++) {
				colors[i] = 255;
				colors[i + 1] = 0;
				colors[i + 2] = 0;
				colors[i + 3] = 255;
				i += 4;
			}
		}


		VkDeviceSize imageSize = width * height * 4;
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = imageSize;
		bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &stagingBuffer) != VK_SUCCESS) {
			printf("Texture Staging Buffer Not Created\n");
		}
		else {
			printf("Texture Buffer Created\n");
		}

		VkMemoryRequirements stagingMemRequirements;
		vkGetBufferMemoryRequirements(logicalDevice, stagingBuffer, &stagingMemRequirements);

		VkMemoryAllocateInfo stagingAllocInfo{};
		stagingAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		stagingAllocInfo.allocationSize = stagingMemRequirements.size;
		stagingAllocInfo.memoryTypeIndex = FindMemoryType(stagingMemRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		if (vkAllocateMemory(logicalDevice, &stagingAllocInfo, nullptr, &stagingBufferMemory) != VK_SUCCESS) {
			printf("Failed to allocated vertex buffer memory\n");
		}
		else {
			printf("Allocated vertex buffer memory\n");
		}

		void* data;
		vkMapMemory(logicalDevice, stagingBufferMemory, 0, imageSize, 0, &data);
		memcpy(data, (void*)&colors[0], static_cast<size_t>(imageSize));
		vkUnmapMemory(logicalDevice, stagingBufferMemory);

		// Creating Image Info
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = static_cast<uint32_t>(width);
		imageInfo.extent.height = static_cast<uint32_t>(height);
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.flags = 0;

		if (vkCreateImage(logicalDevice, &imageInfo, nullptr, &m_TextureImage) != VK_SUCCESS) {
			printf("Texture Image Not Created\n");
		}
		else {
			printf("Texture Image Created\n");
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(logicalDevice, m_TextureImage, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		if (vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &m_TextureImageMemory) != VK_SUCCESS) {
			printf("Texture Image Memory Not Allocated");
		}

		vkBindImageMemory(logicalDevice, m_TextureImage, m_TextureImageMemory, 0);
		vkBindBufferMemory(logicalDevice, stagingBuffer, stagingBufferMemory, 0);

		TransitionImageLayout(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		CopyBufferToImage(stagingBuffer, m_TextureImage, static_cast<uint32_t>(width), static_cast<uint32_t>(height));

		TransitionImageLayout(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		// Create Texture Image View
		CreateTextureImageView();

		// Create Texture Sampler
		CreateTextureSampler();

		// Set Up image Descriptor Info
		m_ImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		m_ImageInfo.imageView = m_TextureImageView;
		m_ImageInfo.sampler = m_TextureSampler;
	}

	void VulkanTexture::UpdateTexture(std::string filename)
	{
	}

	void VulkanTexture::UpdateTexture(Texture* texture)
	{
	}

	VkCommandBuffer VulkanTexture::BeginSingleTimeCommands()
	{
		Application& app = Application::Get();
		VkCommandPool commandPool = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetSwapChain().GetCommandPool();
		VkDevice logicalDevice = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetDevice().GetLogicalDevice();

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = commandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(logicalDevice, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	void VulkanTexture::EndSingleTimeCommands(VkCommandBuffer commandBuffer)
	{
		Application& app = Application::Get();
		VkCommandPool commandPool = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetSwapChain().GetCommandPool();
		VulkanDevice logicalDevice = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetDevice();

		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		VkFenceCreateInfo fenceCreateInfo{};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = 0;

		VkFence fence;

		if (vkCreateFence(logicalDevice.GetLogicalDevice(), &fenceCreateInfo, nullptr, &fence) != VK_SUCCESS){
			printf("Single time fence not created\n");
		}

		vkQueueSubmit(logicalDevice.GetGraphicsQueue(), 1, &submitInfo, fence);
		
		vkWaitForFences(logicalDevice.GetLogicalDevice(), 1, &fence, VK_TRUE, UINT64_MAX);
		
		vkDestroyFence(logicalDevice.GetLogicalDevice(), fence, nullptr);

		vkFreeCommandBuffers(logicalDevice.GetLogicalDevice(), commandPool, 1, &commandBuffer);
	}

	void VulkanTexture::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
	{
		VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

		VkBufferCopy copyRegion{};
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		EndSingleTimeCommands(commandBuffer);
	}

	void VulkanTexture::CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
	{
		VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = {
			width,
			height,
			1
		};

		vkCmdCopyBufferToImage(
			commandBuffer,
			buffer,
			image,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1,
			&region
		);


		EndSingleTimeCommands(commandBuffer);
	}

	void VulkanTexture::CreateTextureImageView()
	{
		m_TextureImageView = CreateImageView(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB);
	}

	void VulkanTexture::CreateTextureSampler() 
	{
		Application& app = Application::Get();
		VulkanDevice logicalDevice = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetDevice();

		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;

		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(logicalDevice.GetVulkanPhysicalDevice().GetPhysicalDevice(), &properties);
		
		samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.0f;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = 0.0f;

		if (vkCreateSampler(logicalDevice.GetLogicalDevice(), &samplerInfo, nullptr, &m_TextureSampler) != VK_SUCCESS) {
			printf("Texture Sampler not paid\n");
		}
	}

	void VulkanTexture::TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
	{
		VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

		VkImageMemoryBarrier barrier{};

		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else {
			printf("unsupported layout transition!\n");
		}

		vkCmdPipelineBarrier(
			commandBuffer,
			sourceStage, destinationStage,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		EndSingleTimeCommands(commandBuffer);
	}

	VkImageView VulkanTexture::CreateImageView(VkImage image, VkFormat format)
	{
		Application& app = Application::Get();
		VulkanDevice logicalDevice = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetDevice();
		
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		VkImageView imageView;

		if (vkCreateImageView(logicalDevice.GetLogicalDevice(), &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
			printf("Texture Image View Not Created\n");
		}

		return imageView;
	}
}