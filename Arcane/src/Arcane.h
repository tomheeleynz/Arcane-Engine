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

/////////////////////////////////////////////////////
////////// Utils
/////////////////////////////////////////////////////
#include "Arcane/Utils/Utils.h"