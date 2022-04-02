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

struct UniformBufferObject
{
	glm::mat4 proj;
	glm::mat4 view;
};

EditorLayer::EditorLayer()
{

}

void EditorLayer::OnAttach()
{
	m_ActiveScene = new Arcane::Scene();

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

	// Working on anticlockwise direction
	// Test Vertices
	std::vector<TestVertex> vertices = { 
		{{-0.5f, -0.5f,0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}, // Top Left
		{{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}}, // Top Right 
		{{0.5f, 0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}, // Bottom Right
		{{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},  // Bottom Left

		{{-0.5f, -0.5f,-0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}, // Top Left
		{{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}}, // Top Right 
		{{0.5f, 0.5f,  -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}, // Bottom Right
		{{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},  //Bottom Left
	};


	std::vector<uint32_t> indices = {
		0, 3, 1, 
		1, 3, 2,
		1, 2, 6, 
		1, 6, 5,
		1, 5, 0,
		5, 4, 0,
		3, 2, 6,
		6, 7, 3,
		4, 7, 3,
		7, 3, 0,
		4, 7, 5,
		7, 6, 5
	};
	
	// Setup framebuffer
	Arcane::FramebufferSpecifications screenFramebufferSpecs;
	
	screenFramebufferSpecs.Height = 512;
	
	screenFramebufferSpecs.Width = 512;
	
	screenFramebufferSpecs.ClearColor = { 0.2f, 0.3f, 0.3f, 1.0f };
	
	screenFramebufferSpecs.AttachmentSpecs = {
		Arcane::FramebufferAttachmentType::COLOR,
		Arcane::FramebufferAttachmentType::DEPTH
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
	m_TestSampler->SetBinding(1);
	m_TestSampler->SetLocation(Arcane::UniformDescriptorLocation::FRAGMENT);

	m_ColorObject = new Arcane::UniformObject(sizeof(UniformBufferObject));
	m_ColorObject->SetBinding(0);
	m_ColorObject->SetLocation(Arcane::UniformDescriptorLocation::VERTEX);

	// Create Uniform Buffer
	m_UniformBuffer = Arcane::UniformBuffer::Create({
		m_ColorObject
	});

	// Test Pipeline
	Arcane::PipelineSpecification spec;
	spec.descriptor = m_VertexDescriptor;
	spec.renderPass = m_RenderPass;
	spec.shader = m_Shader;
	spec.uniformBuffer = m_UniformBuffer;

	m_Pipeline = Arcane::Pipeline::Create(spec);
	m_Viewport = Arcane::UI::AddTexture(m_ScreenFramebuffer);
	m_ViewportSize = {0, 0};

	m_ScenePanel = new ScenePanel();
	m_ScenePanel->SetContext(m_ActiveScene);

	
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnUpdate(float deltaTime)
{
	UniformBufferObject cameraObject;
	
	cameraObject.proj = glm::perspective(glm::radians(45.0f), 1600.0f / 1200.0f, 0.1f, 10.0f);
	cameraObject.view = glm::lookAt(glm::vec3(1.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	
	m_ColorObject->WriteData((void*)&cameraObject);
	m_UniformBuffer->WriteData(m_ColorObject);

	m_ActiveScene->OnUpdate();

	// Geometry Pass (is actually getting rendererd)
	{
		Arcane::Renderer::BeginRenderPass(m_RenderPass);

		Arcane::Renderer::RenderQuad(m_VertexBuffer, m_Pipeline, m_UniformBuffer);

		Arcane::Renderer::EndRenderPass(m_RenderPass);
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
	
	// Compare framebuffer size to viewport size, resize if different
	if ((m_ScreenFramebuffer->GetSpecs().Width != m_ViewportSize.x || 
		m_ScreenFramebuffer->GetSpecs().Height != m_ViewportSize.y) && m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f)
	{
		// m_ScreenFramebuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);
	}

	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	ImGui::BeginMenuBar();
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open"))
				OpenScene();

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	m_ScenePanel->Update();

	ImGui::Begin("Entity Panel");
	{
	
	}
	ImGui::End();

	ImGui::Begin("File Browser");
	{
		
	}
	ImGui::End();
	

	ImGui::Begin("Viewport");
	{
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();

		m_ViewportSize = { viewportSize.x, viewportSize.y };
		
		Arcane::UI::Image(m_Viewport, viewportSize);
	}
	ImGui::End();

	
	//End Dockspace
	ImGui::End();
}

void EditorLayer::OpenScene()
{
	std::string filename = Arcane::FileDialog::OpenFile();

	if (!filename.empty()) {
		Arcane::SceneDeserializer deserializer(filename);
		m_ActiveScene = deserializer.Deserialize();
		m_ScenePanel->SetContext(m_ActiveScene);
	}
}

void EditorLayer::SaveScene()
{

}