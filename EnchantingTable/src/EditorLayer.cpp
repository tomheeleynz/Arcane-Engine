#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <backends/imgui_impl_vulkan.h>
#include <Arcane/Platform/Vulkan/VulkanFramebuffer.h>

#include "EditorLayer.h"

struct TestVertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texCoord;
};

struct ScreenVertex
{
	glm::vec3 position;
	glm::vec2 texCoord;
};

struct UniformBufferObject
{
	glm::vec3 color;
};

EditorLayer::EditorLayer()
{

}

void EditorLayer::OnAttach()
{
	//////////////////////////////////////////////////////////
	//// Geometry Renderpass
	//////////////////////////////////////////////////////////
	m_Shader = Arcane::Shader::Create(
		".\\src\\Assets\\Shaders\\vert.spv", 
		".\\src\\Assets\\Shaders\\frag.spv"
	);

	m_Texture = Arcane::Texture::Create(".\\src\\Assets\\Textures\\shield.png");

	// Test Vertex Descriptor
	m_VertexDescriptor = Arcane::VertexDescriptor::Create({
		Arcane::VertexType::float3,
		Arcane::VertexType::float3,
		Arcane::VertexType::float2
	});

	// Test Vertices
	std::vector<TestVertex> vertices = { 
		{{-0.5f, -0.5f,0.0f}, {1.0f, 0.5f, 0.2f}, {1.0f, 0.0f}},
		{{0.5f, -0.5f, 0.0f}, {1.0f, 0.5f, 0.2f}, {0.0f, 0.0f}},
		{{0.5f, 0.5f,  0.0f}, {1.0f, 0.5f, 0.2f}, {0.0f, 1.0f}},
		{{-0.5f, 0.5f, 0.0f}, {1.0f, 0.5f, 0.2f}, {1.0f, 1.0f}}
	};


	std::vector<uint32_t> indices = {
		0, 1, 2, 2, 3, 0
	};
	
	// Setup framebuffer
	Arcane::FramebufferSpecifications screenFramebufferSpecs;
	
	screenFramebufferSpecs.Height = 512;
	
	screenFramebufferSpecs.Width = 512;
	
	screenFramebufferSpecs.ClearColor = { 1.0f, 1.0f, 0.0f, 1.0f };
	
	screenFramebufferSpecs.AttachmentSpecs = {
		Arcane::FramebufferAttachmentType::COLOR
	};

	m_ScreenFramebuffer = Arcane::Framebuffer::Create(screenFramebufferSpecs);

	// Test Vertex Buffer
	m_VertexBuffer = Arcane::VertexBuffer::Create(vertices.data(), sizeof(TestVertex) * vertices.size());
	Arcane::IndexBuffer* indexBuffer = Arcane::IndexBuffer::Create(indices.data(), indices.size());
	m_VertexBuffer->AddIndexBuffer(indexBuffer);

	Arcane::RenderPassSpecs renderPassSpecs;
	renderPassSpecs.SwapchainFramebuffer = false;
	renderPassSpecs.TargetFramebuffer = m_ScreenFramebuffer;
	m_RenderPass = Arcane::RenderPass::Create(renderPassSpecs);

	m_TestSampler = new Arcane::TextureSampler(m_Texture);
	m_TestSampler->SetBinding(0);

	// Create Uniform Buffer
	m_UniformBuffer = Arcane::UniformBuffer::Create({
		m_TestSampler
	});

	// Test Pipeline
	Arcane::PipelineSpecification spec;
	spec.descriptor = m_VertexDescriptor;
	spec.renderPass = m_RenderPass;
	spec.shader = m_Shader;
	spec.uniformBuffer = m_UniformBuffer;

	m_Pipeline = Arcane::Pipeline::Create(spec);
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////
	//// Screen Renderpass
	//////////////////////////////////////////////////////////
	m_ScreenSpaceShader = Arcane::Shader::Create(
		".\\src\\Assets\\Shaders\\ScreenShaderVert.spv",
		".\\src\\Assets\\Shaders\\ScreenShaderFrag.spv"
	);

	m_ScreenVertexDescriptor = Arcane::VertexDescriptor::Create({
		Arcane::VertexType::float3,
		Arcane::VertexType::float2
	});

	std::vector<ScreenVertex> screenVertices = {
		{{-0.5f, -0.5f,0.0f}, {1.0f, 0.0f}},
		{{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}},
		{{0.5f, 0.5f,  0.0f}, {0.0f, 1.0f}},
		{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f}}
	};

	std::vector<uint32_t> screenIndices = {
		0, 1, 2, 
		2, 3, 0
	};

	// Setting up vertex and index buffers
	m_ScreenVertexBuffer = Arcane::VertexBuffer::Create(screenVertices.data(), screenVertices.size() * sizeof(ScreenVertex));
	m_ScreenIndexBuffer = Arcane::IndexBuffer::Create(screenIndices.data(), screenIndices.size());
	m_ScreenVertexBuffer->AddIndexBuffer(m_ScreenIndexBuffer);

	// Set Up Renderpass 
	Arcane::RenderPassSpecs screenRenderPassSpecs;
	screenRenderPassSpecs.SwapchainFramebuffer = true;
	m_ScreenRenderPass = Arcane::RenderPass::Create(screenRenderPassSpecs);

	// Setup Uniform Buffer
	m_FramebufferSampler = new Arcane::TextureSampler(m_ScreenFramebuffer);
	m_FramebufferSampler->SetBinding(0);

	m_ScreenUniformBuffer = Arcane::UniformBuffer::Create({
		m_FramebufferSampler
	});

	// Create Pipeline
	Arcane::PipelineSpecification screenPipelineSpecs;
	screenPipelineSpecs.descriptor = m_ScreenVertexDescriptor;
	screenPipelineSpecs.renderPass = m_ScreenRenderPass;
	screenPipelineSpecs.shader = m_ScreenSpaceShader;
	screenPipelineSpecs.uniformBuffer = m_ScreenUniformBuffer;
	m_ScreenPipeline = Arcane::Pipeline::Create(screenPipelineSpecs);
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnUpdate(float deltaTime)
{
	// Geometry Pass (is actually getting rendererd)
	{
		Arcane::Renderer::BeginRenderPass(m_RenderPass);

		Arcane::Renderer::RenderQuad(m_VertexBuffer, m_Pipeline, m_UniformBuffer);

		Arcane::Renderer::EndRenderPass(m_RenderPass);
	}
	
	// Screen Space Pass
	{
		Arcane::Renderer::BeginRenderPass(m_ScreenRenderPass);

		Arcane::Renderer::RenderQuad(m_ScreenVertexBuffer, m_ScreenPipeline, m_ScreenUniformBuffer);

		Arcane::Renderer::EndRenderPass(m_ScreenRenderPass);
	}
}

void EditorLayer::OnImGuiRender()
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////// Dock Space Setup
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static bool dockspaceOpen = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	ImGuiIO& io = ImGui::GetIO();

	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	ImGui::Begin("Scene Panel");
	{
	}
	ImGui::End();

	ImGui::Begin("Entity Panel");
	{
	
	}
	ImGui::End();

	ImGui::Begin("File Browser");
	{

	}
	ImGui::End();
	
	
	//End Dockspace
	ImGui::End();
}
