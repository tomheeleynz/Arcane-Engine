#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <Kinetics/Structures/PhysicsBody.h>

#include "PxPhysicsAPI.h"

#include "Arcane/Core/UUID.h"
#include "Arcane/Renderer/Mesh.h"
#include "Arcane/Scripting/Script.h"
#include "Arcane/Physics/RigidBody.h"
#include "Arcane/Physics/BoxCollider.h"
#include "Arcane/Renderer/Camera.h"
#include "Arcane/Renderer/Quad.h"

namespace Arcane
{
	enum class LightType
	{
		DIRECTIONAL = 0,
		POINT
	};

	struct IDComponent
	{
		Core::UUID uuid;
	};

	struct TagComponent
	{
		std::string tag;
	};

	enum class CameraType
	{
		Orthographic,
		Perspective
	};

	struct CameraComponent
	{
		bool isPrimary = false;
		OrthoCamera* orthoCamera;
		PerspectiveCamera* perspectiveCamera;
		CameraType type = CameraType::Perspective;
	};

	struct TransformComponent
	{
		glm::vec3 pos = {0.0f, 0.0f, 0.0f};
		glm::vec3 rotation = { 0.0, 0.0, 0.0 };
		glm::vec3 scale = {1.0f, 1.0f, 1.0f };
	};

	struct MeshComponent
	{
		Mesh* mesh = nullptr;
		std::string filepath;
	};

	struct MeshRendererComponent
	{
		Material* material;
	};

	struct LightComponent
	{
		LightType type;
		glm::vec3 color;
	};

	struct ScriptComponent
	{
		Script* script;
	};

	struct RigidBodyComponent
	{
		RigidBody* rigidBody;
	};

	struct BoxColliderComponent
	{
		BoxCollider* boxCollider;
	};

	struct SpriteRenderer
	{
		Texture* sprite;
		glm::vec3 color;
		Material* material;
		Quad* quad;
	};

	struct RigidBody2DComponent 
	{
		Kinetics::Shape* m_Shape;
		Kinetics::PhysicsBody* body;
		glm::vec2 velocity;
		float gravityScale;
	};
}