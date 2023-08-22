#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <Kinetics/Core/DynamicBody.h>

#include "Arcane/Core/UUID.h"
#include "Arcane/Renderer/Mesh.h"
#include "Arcane/Scripting/Script.h"
#include "Arcane/Renderer/Camera.h"
#include "Arcane/Animation/AnimationController.h"

namespace Arcane
{
	enum class LightType
	{
		DIRECTIONAL = 0,
		POINT
	};

	enum class CameraType
	{
		Orthographic,
		Perspective
	};

	struct IDComponent
	{
		Core::UUID uuid;
	};

	struct TagComponent
	{
		std::string tag;
	};

	struct CameraComponent
	{
		bool isPrimary = false;
		OrthoCamera* orthoCamera = nullptr;
		PerspectiveCamera* perspectiveCamera = nullptr;
		CameraType type = CameraType::Orthographic;
	};

	struct TransformComponent
	{
		glm::vec3 pos = { 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation = { 0.0, 0.0, 0.0 };
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
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
		bool updateProperties = true;
	};

	struct SpriteRendererComponent
	{
		glm::vec3 color = { 1.0f, 1.0f, 1.0f };
		Texture* sprite = nullptr;
		bool flipX = false;
		bool flipY = false;
	};

	struct RigidBodyComponent2D
	{
		Kinetics::DynamicBody2D* body;
		float mass;
		float gravityScale;
	};

	struct BoxCollider2D
	{
		Kinetics::AABB boundingBox;
	};

	///////////////////////
	/// Animation
	//////////////////////
	struct Animator
	{
		AnimationController* controller = nullptr;
	};
}