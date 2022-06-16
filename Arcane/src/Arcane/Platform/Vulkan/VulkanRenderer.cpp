#include <iostream>
#include "VulkanRenderer.h"
#include "VulkanContext.h"
#include "VulkanPipeline.h"
#include "VulkanBuffer.h"
#include "VulkanUniformBuffer.h"
#include "VulkanRenderPass.h"
#include "VulkanFramebuffer.h"
#include "Arcane/Core/Application.h"
#include "VulkanSet.h"

namespace Arcane {

	VulkanRenderer::VulkanRenderer()
	{
		Init();
	}

	void VulkanRenderer::Init()
	{
		Application& app = Application::Get();
		VulkanContext* _context = static_cast<VulkanContext*>(app.GetWindow().GetContext());
	}

	void VulkanRenderer::Shutdown()
	{
		Application& app = Application::Get();
		VulkanContext* _context = static_cast<VulkanContext*>(app.GetWindow().GetContext());
		vkDeviceWaitIdle(_context->GetDevice().GetLogicalDevice());
	}


	void VulkanRenderer::BeginFrame()
	{
		Application& app = Application::Get();
		VulkanContext* _context = static_cast<VulkanContext*>(app.GetWindow().GetContext());
		VulkanSwapChain& swapChain = _context->GetSwapChain();
		auto swapChainCommandBuffers = swapChain.GetCommandBuffers();

		for (size_t i = 0; i < swapChainCommandBuffers.size(); i++)
		{
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = 0;
			beginInfo.pInheritanceInfo = nullptr;

			if (vkBeginCommandBuffer(swapChainCommandBuffers[i], &beginInfo) != VK_SUCCESS) {
				printf("Command Buffer Not Began\n");
			}
		}
	}

	void VulkanRenderer::EndFrame()
	{
		Application& app = Application::Get();
		VulkanContext* _context = static_cast<VulkanContext*>(app.GetWindow().GetContext());
		VulkanSwapChain& swapChain = _context->GetSwapChain();
		auto swapChainCommandBuffers = swapChain.GetCommandBuffers();

		for (size_t i = 0; i < swapChainCommandBuffers.size(); i++)
		{
			if (vkEndCommandBuffer(swapChainCommandBuffers[i]) != VK_SUCCESS) {
				printf("Command Buffer End Failed\n");
			}
		}
	}

	void VulkanRenderer::BeginRenderPass(RenderPass* renderPass)
	{
		bool renderToSwapchain = false;

		Application& app = Application::Get();
		VulkanContext* _context = static_cast<VulkanContext*>(app.GetWindow().GetContext());
		VulkanSwapChain& swapChain = _context->GetSwapChain();

		RenderPassSpecs& renderSpecs = renderPass->GetRenderPassSpecs();
		
		if (renderSpecs.SwapchainFramebuffer) {		
			renderToSwapchain = true;
		}

		VulkanFramebuffer* frameBuffer = static_cast<VulkanFramebuffer*>(renderSpecs.TargetFramebuffer);

		auto swapChainCommandBuffers = swapChain.GetCommandBuffers();
		auto swapChainFramebuffers = swapChain.GetSwapChainFramebuffers();

		for (size_t i = 0; i < swapChainCommandBuffers.size(); i++)
		{
			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;

			// Set Viewport
			VkViewport viewport;
			viewport.x = 0;
			viewport.y = 0;
			viewport.maxDepth = 1.0f;
			viewport.minDepth = 0.0f;

			// Set Scissor
			VkRect2D scissor;
			scissor.offset = {0, 0};

			if (renderToSwapchain) {
				renderPassInfo.renderPass = swapChain.GetSwapchainRenderPass();
				renderPassInfo.framebuffer = swapChainFramebuffers[i];
				renderPassInfo.renderArea.extent = swapChain.GetExtent();

				viewport.height = swapChain.GetExtent().height;
				viewport.width = swapChain.GetExtent().width;

				scissor.extent = swapChain.GetExtent();
			}
			else {
				renderPassInfo.renderPass = frameBuffer->GetFramebufferRenderPass();
				renderPassInfo.framebuffer = frameBuffer->GetVulkanFramebuffer();
				renderPassInfo.renderArea.extent = { frameBuffer->GetSpecs().Width, frameBuffer->GetSpecs().Height};

				viewport.height = frameBuffer->GetSpecs().Height;
				viewport.width = frameBuffer->GetSpecs().Width;

				scissor.extent = { frameBuffer->GetSpecs().Width, frameBuffer->GetSpecs().Height };
			}
			
			renderPassInfo.renderArea.offset = { 0, 0 };

			std::array<VkClearValue, 2> clearValues;

			if (renderToSwapchain) {
				clearValues[0].color = { { 0.2f, 0.3f, 0.3f, 1.0f }};
			}
			else {
				clearValues[0].color = {{
					frameBuffer->GetSpecs().ClearColor.r,
					frameBuffer->GetSpecs().ClearColor.g,
					frameBuffer->GetSpecs().ClearColor.b,
					frameBuffer->GetSpecs().ClearColor.a
				}};
			}

			clearValues[1].depthStencil = {1.0f, 0};

			

			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();
			vkCmdBeginRenderPass(swapChainCommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			// Set Viewport
			vkCmdSetViewport(swapChainCommandBuffers[i], 0, 1, &viewport);

			// Set Scissor
			vkCmdSetScissor(swapChainCommandBuffers[i], 0, 1, &scissor);
		}
	}

	void VulkanRenderer::EndRenderPass(RenderPass* renderPass)
	{
		Application& app = Application::Get();
		VulkanContext* _context = static_cast<VulkanContext*>(app.GetWindow().GetContext());
		VulkanSwapChain& swapChain = _context->GetSwapChain();

		auto swapChainCommandBuffers = swapChain.GetCommandBuffers();

		for (size_t i = 0; i < swapChainCommandBuffers.size(); i++)
		{
			vkCmdEndRenderPass(swapChainCommandBuffers[i]);
		}
	}

	void VulkanRenderer::RenderTriangle(VertexBuffer* buffer, Pipeline* pipeline)
	{
		Application& app = Application::Get();

		VulkanContext* _context = static_cast<VulkanContext*>(app.GetWindow().GetContext());
		VulkanSwapChain& swapChain = _context->GetSwapChain();

		// Bind Pipeline for triangle to use
		VkPipeline vulkanPipeline = static_cast<VulkanPipeline*>(pipeline)->GetPipeline();
		VkBuffer vulkanVertexBuffer = static_cast<VulkanVertexBuffer*>(buffer)->GetVertexBuffer();
		
		std::vector<VkCommandBuffer> swapChainBuffers = swapChain.GetCommandBuffers();

		for (size_t i = 0; i < swapChainBuffers.size(); i++) {
			// Bind Pipeline
			vkCmdBindPipeline(swapChainBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline);

			// Bind Vertex Buffer
			VkBuffer vertexBuffers[] = { vulkanVertexBuffer };
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(swapChainBuffers[i], 0, 1, vertexBuffers, offsets);

			// Draw Vertices
			vkCmdDraw(swapChainBuffers[i], 3, 1, 0, 0);
		}
	}

	void VulkanRenderer::RenderQuad(VertexBuffer* buffer, Pipeline* pipeline)
	{
		Application& app = Application::Get();

		VulkanContext* _context = static_cast<VulkanContext*>(app.GetWindow().GetContext());
		VulkanSwapChain& swapChain = _context->GetSwapChain();

		// Bind Pipeline for triangle to use
		VulkanPipeline* vulkanPipeline = static_cast<VulkanPipeline*>(pipeline);
		VkBuffer vulkanVertexBuffer = static_cast<VulkanVertexBuffer*>(buffer)->GetVertexBuffer();

		// Get Index Buffer and count from vertex buffer
		IndexBuffer* indexBuffer = buffer->GetIndexBuffer();
		uint32_t indiceCount = indexBuffer->GetCount();
		VkBuffer vulkanIndexBuffer = static_cast<VulkanIndexBuffer*>(indexBuffer)->GetIndexBuffer();

		std::vector<VkCommandBuffer> swapChainBuffers = swapChain.GetCommandBuffers();

		for (size_t i = 0; i < swapChainBuffers.size(); i++) {
			// Bind Pipeline
			vkCmdBindPipeline(swapChainBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline->GetPipeline());

			// Bind Vertex Buffer
			VkBuffer vertexBuffers[] = { vulkanVertexBuffer };
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(swapChainBuffers[i], 0, 1, vertexBuffers, offsets);

			// Bind Index Buffer
			vkCmdBindIndexBuffer(swapChainBuffers[i], vulkanIndexBuffer, 0, VK_INDEX_TYPE_UINT32);

			// Draw Vertices
			vkCmdDrawIndexed(swapChainBuffers[i], indiceCount, 1, 0, 0, 0);
		}
	}


	void VulkanRenderer::RenderMesh(VertexBuffer* buffer, Pipeline* pipeline, DescriptorSet* descriptorSet)
	{
		Application& app = Application::Get();

		VulkanContext* _context = static_cast<VulkanContext*>(app.GetWindow().GetContext());
		VulkanSwapChain& swapChain = _context->GetSwapChain();

		// Bind Pipeline for triangle to use
		VulkanPipeline* vulkanPipeline = static_cast<VulkanPipeline*>(pipeline);
		VkBuffer vulkanVertexBuffer = static_cast<VulkanVertexBuffer*>(buffer)->GetVertexBuffer();

		// Get Index Buffer and count from vertex buffer
		IndexBuffer* indexBuffer = buffer->GetIndexBuffer();
		uint32_t indiceCount = indexBuffer->GetCount();
		VkBuffer vulkanIndexBuffer = static_cast<VulkanIndexBuffer*>(indexBuffer)->GetIndexBuffer();

		// VulkanUniformBuffer* vulkanUniformBuffer = static_cast<VulkanUniformBuffer*>(uniformBuffer);
		VulkanSet* vulkanSet = static_cast<VulkanSet*>(descriptorSet);

		std::vector<VkCommandBuffer> swapChainBuffers = swapChain.GetCommandBuffers();

		for (size_t i = 0; i < swapChainBuffers.size(); i++) {
			// Bind Pipeline
			vkCmdBindPipeline(swapChainBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline->GetPipeline());

			// Bind Vertex Buffer
			VkBuffer vertexBuffers[] = { vulkanVertexBuffer };
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(swapChainBuffers[i], 0, 1, vertexBuffers, offsets);

			// Bind Descriptor Sets
			vkCmdBindDescriptorSets(swapChainBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline->GetLayout(), 0, 1, &vulkanSet->GetDescriptorSets()[i], 0, nullptr);

			// Bind Index Buffer
			vkCmdBindIndexBuffer(swapChainBuffers[i], vulkanIndexBuffer, 0, VK_INDEX_TYPE_UINT32);

			// Draw Vertices
			vkCmdDrawIndexed(swapChainBuffers[i], indiceCount, 1, 0, 0, 0);
		}
	}

	void VulkanRenderer::RenderQuad(VertexBuffer* buffer, Pipeline* pipeline, DescriptorSet* descriptorSet)
	{
		Application& app = Application::Get();

		VulkanContext* _context = static_cast<VulkanContext*>(app.GetWindow().GetContext());
		VulkanSwapChain& swapChain = _context->GetSwapChain();

		// Bind Pipeline for triangle to use
		VulkanPipeline* vulkanPipeline = static_cast<VulkanPipeline*>(pipeline);
		VkBuffer vulkanVertexBuffer = static_cast<VulkanVertexBuffer*>(buffer)->GetVertexBuffer();

		// Get Index Buffer and count from vertex buffer
		IndexBuffer* indexBuffer = buffer->GetIndexBuffer();
		uint32_t indiceCount = indexBuffer->GetCount();
		VkBuffer vulkanIndexBuffer = static_cast<VulkanIndexBuffer*>(indexBuffer)->GetIndexBuffer();

		VulkanSet* vulkanSet = static_cast<VulkanSet*>(descriptorSet);
		// VulkanUniformBuffer* vulkanUniformBuffer = static_cast<VulkanUniformBuffer*>(uniformBuffer);

		std::vector<VkCommandBuffer> swapChainBuffers = swapChain.GetCommandBuffers();

		for (size_t i = 0; i < swapChainBuffers.size(); i++) {
			// Bind Pipeline
			vkCmdBindPipeline(swapChainBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline->GetPipeline());

			// Bind Vertex Buffer
			VkBuffer vertexBuffers[] = { vulkanVertexBuffer };
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(swapChainBuffers[i], 0, 1, vertexBuffers, offsets);
			
			// Bind Descriptor Sets
			vkCmdBindDescriptorSets(swapChainBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline->GetLayout(), 0, 1, &vulkanSet->GetDescriptorSets()[i], 0, nullptr);

			// Bind Index Buffer
			vkCmdBindIndexBuffer(swapChainBuffers[i], vulkanIndexBuffer, 0, VK_INDEX_TYPE_UINT32);

			// Draw Vertices
			vkCmdDrawIndexed(swapChainBuffers[i], indiceCount, 1, 0, 0, 0);
		}
	}
}