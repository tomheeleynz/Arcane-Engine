#include "Arcane/Core/Application.h"

#include "VulkanPipeline.h"
#include "VulkanShader.h"
#include "VulkanContext.h"
#include "VulkanRenderPass.h"
#include "VulkanVertexDescriptor.h"
#include "VulkanUniformBuffer.h"
#include "VulkanFramebuffer.h"

namespace Arcane {
	VulkanPipeline::VulkanPipeline(PipelineSpecification& spec)
	{
		VulkanShader* shader = static_cast<VulkanShader*>(spec.shader);

		// Get Vulkan Context to be able to get the devices
		Application& app = Application::Get();
		Window& window = app.GetWindow();
		VulkanContext* _context = static_cast<VulkanContext*>(window.GetContext());
		VkDevice logicalDevice = _context->GetDevice().GetLogicalDevice();
		
		// Get Vertex Binding Descriptions
		VkVertexInputBindingDescription bindingDescription = static_cast<VulkanVertexDescriptor*>(spec.descriptor)->GetBindingDescription();
		auto attributeDescriptions = static_cast<VulkanVertexDescriptor*>(spec.descriptor)->GetAttributeDescriptions();

		// Creating Vertex Shader
		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageInfo.module = shader->GetVertexShaderModule();
		vertShaderStageInfo.pName = "main";

		// Create Fragment Shader
		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = shader->GetFragmentShaderModule();
		fragShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

		// Possibly be vertex descriptor at some point
		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		// What type of shape to draw
		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		// If the pipeline is using an custom framebuffer, set these values tpo those

		// Set Viewport
		VkViewport viewport{};
		VkRect2D scissor{};
		
		if (spec.renderPass->GetRenderPassSpecs().SwapchainFramebuffer == false) {
			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = spec.renderPass->GetRenderPassSpecs().TargetFramebuffer->GetSpecs().Width;
			viewport.height = spec.renderPass->GetRenderPassSpecs().TargetFramebuffer->GetSpecs().Height;
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;

			// Set Scissor
			scissor.offset = { 0, 0 };
			scissor.extent = {
				spec.renderPass->GetRenderPassSpecs().TargetFramebuffer->GetSpecs().Width,
				spec.renderPass->GetRenderPassSpecs().TargetFramebuffer->GetSpecs().Height
			};
		}
		else {
			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = (float)_context->GetSwapChain().GetExtent().width;
			viewport.height = (float)_context->GetSwapChain().GetExtent().height;
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;

			// Set Scissor
			scissor.offset = { 0, 0 };
			scissor.extent = _context->GetSwapChain().GetExtent();
		}

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;

		// Rasterizer
		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;

		// Multisampling
		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		// Color Blend Attachment
		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_TRUE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		// Color Blending
		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

		if (spec.uniformBuffer != nullptr) {
			auto vulkanUniformBufferLayout = static_cast<VulkanUniformBuffer*>(spec.uniformBuffer)->GetLayout();
			pipelineLayoutInfo.setLayoutCount = 1;
			pipelineLayoutInfo.pSetLayouts = &vulkanUniformBufferLayout;
		}

		if (vkCreatePipelineLayout(logicalDevice, &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS) {
			printf("Pipeline Layout Not Created");
		}
		else {
			printf("Pipeline Layout Created\n");
		}


		VkRenderPass renderPass = VK_NULL_HANDLE;

		if (spec.renderPass->GetRenderPassSpecs().SwapchainFramebuffer)
		{
			renderPass = _context->GetSwapChain().GetSwapchainRenderPass();
		}
		else {
			VulkanFramebuffer* frameBuffer = static_cast<VulkanFramebuffer*>(spec.renderPass->GetRenderPassSpecs().TargetFramebuffer);
			renderPass = frameBuffer->GetFramebufferRenderPass();
		}

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = nullptr; // Optional
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = nullptr; // Optional
		pipelineInfo.layout = m_PipelineLayout;
		pipelineInfo.renderPass = renderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
		pipelineInfo.basePipelineIndex = -1; // Optional

		if (vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_Pipeline) != VK_SUCCESS) {
			printf("Pipeline Not Created");
		}
		else {
			printf("Pipeline Created\n");
		}
	}
}