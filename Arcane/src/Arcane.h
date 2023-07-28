#pragma once

/////////////////////////////////////////////////////
////////// Core
/////////////////////////////////////////////////////
#include "Arcane/Core/Application.h"
#include "Arcane/Core/Layer.h"
#include "Arcane/Core/InputManager.h"

/////////////////////////////////////////////////////
////////// Rendering
/////////////////////////////////////////////////////
#include "Arcane/Renderer/Renderer.h"
#include "Arcane/Renderer/Pipeline.h"
#include "Arcane/Renderer/Shader.h"
#include "Arcane/Renderer/RenderPass.h"
#include "Arcane/Renderer/VertexDescriptor.h"
#include "Arcane/Renderer/Buffer.h"
#include "Arcane/Renderer/UniformBuffer.h"
#include "Arcane/Renderer/Texture.h"
#include "Arcane/Renderer/Framebuffer.h"
#include "Arcane/Renderer/SceneRenderer.h"
#include "Arcane/Renderer/Camera.h"
#include "Arcane/Renderer/DescriptorSet.h"
#include "Arcane/Renderer/MeshFactory.h"
#include "Arcane/Renderer/MaterialSerializer.h"
#include "Arcane/Renderer/SkyBox.h"

/////////////////////////////////////////////////////
////////// ImGui
/////////////////////////////////////////////////////
#include "Arcane/ImGui/ImGuiLayer.h"
#include "Arcane/ImGui/PlatformImGui.h"

/////////////////////////////////////////////////////
////////// Scene and ECS
/////////////////////////////////////////////////////
#include "Arcane/ECS/Entity.h"
#include "Arcane/Scene/Scene.h"
#include "Arcane/Scene/SceneDeserializer.h"
#include "Arcane/Scene/SceneSerializer.h"

/////////////////////////////////////////////////////
////////// Utils
/////////////////////////////////////////////////////
#include "Arcane/Utils/Utils.h"
#include "Arcane/Utils/FileWatcher.h"

/////////////////////////////////////////////////////
////////// Scripting
/////////////////////////////////////////////////////
#include "Arcane/Scripting/ScriptingEngine.h"

/////////////////////////////////////////////////////
////////// Animation
/////////////////////////////////////////////////////
#include "Arcane/Animation/Animation.h"
#include "Arcane/Animation/AnimationController.h"
#include "Arcane/Animation/AnimationSerializer.h"