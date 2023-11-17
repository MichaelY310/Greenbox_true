#pragma once

#include "Render/Texture2D.h"
#include "Render/Camera.h"

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"

namespace Greenbox {


	struct NameComponent
	{
		std::string Name = "";

		NameComponent() = default;
		NameComponent(const NameComponent&) = default;
		NameComponent(const std::string& name) : Name(name) {}
	};


	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		glm::vec3 TempTranslation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 TempRotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 TempScale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translate, 
			const glm::vec3& rotation, 
			const glm::vec3& scale)
			: Translation(translate), Rotation(rotation), Scale(scale) {}

		glm::mat4 GetTransform() 
		{ 
			return glm::translate(glm::mat4(1.0f), Translation) *
				glm::toMat4(glm::quat(Rotation)) *
				glm::scale(glm::mat4(1.0f), Scale);
		}
	};


	struct SpriteRendererComponent
	{
		glm::vec4 Color = { 1.0, 1.0, 1.0, 1.0 };
		std::shared_ptr<Texture2D> Texture;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color) : Color(color) {}
	};

	struct QuadRendererComponent
	{
		glm::vec4 Color = { 1.0, 1.0, 1.0, 1.0 };
		std::shared_ptr<Texture2D> Texture;

		QuadRendererComponent() = default;
		QuadRendererComponent(const QuadRendererComponent&) = default;
		QuadRendererComponent(const glm::vec4& color) : Color(color) {}


	};

	struct TriangleRendererComponent
	{
		glm::vec4 Color = { 1.0, 1.0, 1.0, 1.0 };
		std::shared_ptr<Texture2D> Texture;

		TriangleRendererComponent() = default;
		TriangleRendererComponent(const TriangleRendererComponent&) = default;
		TriangleRendererComponent(const glm::vec4& color) : Color(color) {}

	};

	struct CameraComponent
	{
		Camera camera;
		bool Primary = true;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct Box2DRigidbodyComponent
	{
		b2BodyType BodyType = b2_staticBody; // b2_staticBody, b2_dynamicBody, b2_kinematicBody
		bool FixedRotation = false;
		b2Body* RuntimeBody = nullptr;

		Box2DRigidbodyComponent() = default;
		Box2DRigidbodyComponent(const Box2DRigidbodyComponent&) = default;
	};

	struct Box2DBoxColliderComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };

		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		b2Fixture* RuntimeFixture = nullptr;

		Box2DBoxColliderComponent() = default;
		Box2DBoxColliderComponent(const Box2DBoxColliderComponent&) = default;
	};
}