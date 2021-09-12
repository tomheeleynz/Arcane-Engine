#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>


#include "EditorLayer.h"

struct TestVertex
{
	glm::vec3 position;
	glm::vec3 color;
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
	// Test Shader
	m_Shader = Arcane::Shader::Create(".\\src\\Assets\\Shaders\\vert.spv", ".\\src\\Assets\\Shaders\\frag.spv");
	m_RenderPass = Arcane::RenderPass::Create();

	// Test Vertex Descriptor
	m_VertexDescriptor = Arcane::VertexDescriptor::Create({
		Arcane::VertexType::float3,
		Arcane::VertexType::float3,
		Arcane::VertexType::float2
	});


	Arcane::Texture* testTexture = Arcane::Texture::Create(".\\src\\Assets\\Textures\\shield.png");
	m_UniformBuffer = Arcane::UniformBuffer::Create(testTexture, sizeof(UniformBufferObject));

	// Test Pipeline
	Arcane::PipelineSpecification spec;
	spec.descriptor = m_VertexDescriptor;
	spec.renderPass = m_RenderPass;
	spec.shader = m_Shader;
	spec.uniformBuffer = m_UniformBuffer;

	m_Pipeline = Arcane::Pipeline::Create(spec);

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

	// Test Vertex Buffer
	m_VertexBuffer = Arcane::VertexBuffer::Create(vertices.data(), sizeof(TestVertex) * vertices.size());
	Arcane::IndexBuffer* indexBuffer = Arcane::IndexBuffer::Create(indices.data(), indices.size());
	m_VertexBuffer->AddIndexBuffer(indexBuffer);

}

void EditorLayer::OnDetach()
{

}

void EditorLayer::OnUpdate(float deltaTime)
{
	UniformBufferObject ubo;
	ubo.color = {1.0f, 0.5f, 0.2f};

	m_UniformBuffer->WriteData(&ubo, sizeof(ubo));

	// Begin a Render pass
	Arcane::Renderer::BeginRenderPass(m_RenderPass);

	// Render Test Triangle
	Arcane::Renderer::RenderQuad(m_VertexBuffer, m_Pipeline, m_UniformBuffer);

	// End a pass
	Arcane::Renderer::EndRenderPass(m_RenderPass);
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


	ImGui::Begin("Hello World");
	{
		if (ImGui::Button("Test Button")) 
		{
			printf("Hello, ImGui Button\n");
		}
	}
	ImGui::End();

	//End Dockspace
	ImGui::End();
}
