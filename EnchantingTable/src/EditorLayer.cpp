#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <backends/imgui_impl_vulkan.h>
#include <Arcane/Platform/Vulkan/VulkanFramebuffer.h>

#include "EditorLayer.h"
#include "Panels/PanelStructs.h"

EditorLayer::EditorLayer()
{
}

void EditorLayer::OnAttach()
{
	m_EditorScene = new Arcane::Scene(true);

	m_SceneRenderer = new Arcane::SceneRenderer();
	
	m_EditorScene->SetSceneRenderer(m_SceneRenderer);

	Arcane::DimensionType dimensionType = Arcane::Application::Get().GetProject()->GetDimensionType();
	
	m_Viewport = Arcane::UI::AddTexture(m_SceneRenderer->GetFinalRenderFramebuffer());

	m_ViewportSize = {0, 0};

	// Create Panels
	m_ScenePanel = new ScenePanel();
	m_ScenePanel->SetContext(m_EditorScene);

	m_EntityPanel = new EntityPanel();
	m_FileBrowserPanel = new FileBrowserPanel();
	m_MaterialViewerPanel = new MaterialViewerPanel();
	m_EnviromentPanel = new EnvironmentPanel();
	m_SpriteEditor = new SpriteEditor();

	m_FileBrowserPanel->SetMaterialViewPanel(m_MaterialViewerPanel);

	// Setup Camera
	if (dimensionType == Arcane::DimensionType::TwoD)
		m_EditorCamera = new Arcane::OrthoCamera(512, 512);
	else
		//m_EditorCamera = new Arcane::OrthoCamera(512, 512);
		m_EditorCamera = new Arcane::PerspectiveCamera(512, 512, 45.0f);
	
	
	m_EditorScene->SetSceneCamera(m_EditorCamera);
	
	// Setup Camera Controller
	if (dimensionType == Arcane::DimensionType::TwoD)
		m_EditorCameraController = new OrthographicController();
	else
		//m_EditorCameraController = new OrthographicController();
		m_EditorCameraController = new PerspectiveController();
	
	m_EditorCameraController->SetCamera(m_EditorCamera);

	m_ActiveScene = m_EditorScene;
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnUpdate(float deltaTime)
{
	Arcane::FramebufferSpecifications fbSpecs = m_SceneRenderer->GetFinalRenderFramebuffer()->GetSpecs();

	if (m_ViewportSize.x > 0.0f &&
		m_ViewportSize.y > 0.0f &&
		(fbSpecs.Width != m_ViewportSize.x || fbSpecs.Height != m_ViewportSize.y))
	{
		m_SceneRenderer->ResizeScene(m_ViewportSize.x, m_ViewportSize.y);

		// Resize Camera
		m_EditorCamera->OnResize(m_ViewportSize.x, m_ViewportSize.y);

		m_Viewport = Arcane::UI::AddTexture(m_SceneRenderer->GetFinalRenderFramebuffer());
	}

	switch (m_State)
	{
	case SceneState::EDIT:
		m_ActiveScene->OnUpdate(deltaTime);
		break;
	case SceneState::PLAY:
		m_ActiveScene->OnRuntimeUpdate(deltaTime);
		break;
	default:
		break;
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


	// Resize Framebuffer if necessary
	ImGui::BeginMenuBar();
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open"))
				OpenScene();

			if (ImGui::MenuItem("Save"))
				SaveScene();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Scene"))
		{
			if (ImGui::MenuItem("Environment"))
				m_ShowEnvironmentPanel = true;

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Tools"))
		{
			if (ImGui::MenuItem("Sprite Editor"))
				m_ShowSpriteEditor = true;

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	m_ScenePanel->Update();

	m_EntityPanel->SetContext(m_ScenePanel->GetSelectedEntity());
	m_EntityPanel->Update();

	m_FileBrowserPanel->OnUpdate();

	if (m_EntityPanel->GetSelectedMaterial() != nullptr)
		m_MaterialViewerPanel->SetMaterial(m_EntityPanel->GetSelectedMaterial());

	m_MaterialViewerPanel->OnUpdate();

	if (m_ShowEnvironmentPanel)
		m_EnviromentPanel->OnImGuiRender();

	if (m_ShowSpriteEditor)
		m_SpriteEditor->OnImGuiRender();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	
	ImGui::Begin("Viewport");
	{
		if (ImGui::IsWindowHovered()) {
			if (m_State == SceneState::EDIT) {
				m_EditorCameraController->OnUpdate();
			}

			if (Arcane::InputManager::GetKeyReleased(32)) {
				if (m_State == SceneState::EDIT) {
					OnScenePlay();
				}
				else if (m_State == SceneState::PLAY) {
					OnSceneStop();
				}
			}
		}

		ImVec2 viewportSize = ImGui::GetContentRegionAvail();

		m_ViewportSize = { viewportSize.x, viewportSize.y };
	
		Arcane::UI::Image(m_Viewport, viewportSize);

		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CURRENT_SELECTED_ASSET");

			if (payload != nullptr) {
				// Get Asset Id
				AssetInfo assetInfo = *static_cast<AssetInfo*>(payload->Data);
				Arcane::Asset* sceneAsset = Arcane::Application::Get().GetAssetDatabase().GetAsset(assetInfo.id);

				if (sceneAsset != nullptr && sceneAsset->GetAssetType() == Arcane::AssetType::SCENE) {
					Arcane::Scene* scene = static_cast<Arcane::Scene*>(sceneAsset);
					m_ActiveScene = scene;
					m_ActiveScene->SetSceneRenderer(m_SceneRenderer);
					m_ScenePanel->SetContext(m_ActiveScene);
				}
			}
			ImGui::EndDragDropTarget();
		}

	}
	ImGui::End();
	
	ImGui::PopStyleVar();

	
	//End Dockspace
	ImGui::End();
}

void EditorLayer::OpenScene()
{
	std::string filename = Arcane::FileDialog::OpenFile();

	if (!filename.empty()) {
		Arcane::SceneDeserializer deserializer(filename);
		m_EditorScene = deserializer.Deserialize();
		m_EditorScene->SetSceneRenderer(m_SceneRenderer);
		m_ScenePanel->SetContext(m_EditorScene);
		m_ActiveScene = m_EditorScene;
	}
}

void EditorLayer::SaveScene()
{
	std::string filename = Arcane::FileDialog::SaveFile();
	
	if (!filename.empty()) {
		Arcane::SceneSerializer serializer(m_ActiveScene);
		serializer.Serialize(filename);
		Arcane::Application::Get().GetAssetDatabase().GenerateAsset(filename, false);
	}
}

void EditorLayer::OnScenePlay()
{
	m_State = SceneState::PLAY;

	m_ActiveScene = Arcane::Scene::Copy(m_EditorScene);
	m_ActiveScene->SetSceneRenderer(m_SceneRenderer);

	m_ActiveScene->OnRuntimeStart();
	
	m_ScenePanel->SetContext(m_ActiveScene);
}

void EditorLayer::OnSceneStop()
{
	m_State = SceneState::EDIT;
	m_ActiveScene = m_EditorScene;
	m_ScenePanel->SetContext(m_ActiveScene);
}