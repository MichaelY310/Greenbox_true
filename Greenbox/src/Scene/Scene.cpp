#include "gbpch.h"

#include <box2d/b2_world.h>

#include "Scene.h"
#include "Render/Renderer.h"
#include "Components.h"
#include "Entity.h"

namespace Greenbox {

	void Scene::OnUpdate(float timestep, Camera& EditorCamera)
	{
		if (m_SceneState == SceneState::Edit)
		{
			if (m_PrevSceneState == SceneState::Play)
				OnPlayEnd();
			OnUpdateEdit(timestep, EditorCamera);
		}
		else if (m_SceneState == SceneState::Play)
		{
			if (m_PrevSceneState == SceneState::Edit)
				OnPlayStart();
			OnUpdatePlay(timestep);
		}
		m_PrevSceneState = m_SceneState;

	}

	void Scene::OnUpdateCore(float timestep)
	{
		{
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer::AddQuad(transform.GetTransform(), sprite.Color, sprite.Texture, (int)entity);
			}
		}

		{
			auto view = m_Registry.view<TransformComponent, QuadRendererComponent>();
			for (auto entity : view)
			{
				auto [transform, quad] = view.get<TransformComponent, QuadRendererComponent>(entity);
				Renderer::AddQuad(transform.GetTransform(), quad.Color, quad.Texture, (int)entity);
			}
		}

		{
			auto view = m_Registry.view<TransformComponent, TriangleRendererComponent>();
			for (auto entity : view)
			{
				auto [transform, triangle] = view.get<TransformComponent, TriangleRendererComponent>(entity);
				Renderer::AddTriangle(transform.GetTransform(), triangle.Color, triangle.Texture, (int)entity);
			}
		}

		Renderer::Draw();
	}

	void Scene::OnUpdateEdit(float timestep, Camera& EditorCamera)
	{
		Renderer::SetCamera(EditorCamera);
		OnUpdateCore(timestep);
	}

	void Scene::OnUpdatePlay(float timestep)
	{
		// Scene Camera
		Camera* primaryCamera = nullptr;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transformComponent, cameraComponent] = view.get<TransformComponent, CameraComponent>(entity);
				Camera* camera = &cameraComponent.camera;
				camera->SetViewportSize(m_ViewportWidth, m_ViewportHeight);
				camera->SetView(glm::inverse(transformComponent.GetTransform()));
				if (cameraComponent.Primary)
				{
					primaryCamera = camera;
				}
			}
		}
		GB_ASSERT(primaryCamera != nullptr, "There is no primary camera!!");

		// Box2D Physics
		const int32_t velocityIterations = 6;
		const int32_t positionIterations = 2;
		m_Box2DPhysicsWorld->Step(timestep, velocityIterations, positionIterations);

		auto view = m_Registry.view<Box2DRigidbodyComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			TransformComponent& transformComponent = entity.GetComponent<TransformComponent>();
			Box2DRigidbodyComponent& rigidBodyComponent = entity.GetComponent<Box2DRigidbodyComponent>();

			const auto& position = rigidBodyComponent.RuntimeBody->GetPosition();
			transformComponent.Translation.x = position.x;
			transformComponent.Translation.y = position.y;
			transformComponent.Rotation.z = rigidBodyComponent.RuntimeBody->GetAngle();
		}

		Renderer::SetCamera(*primaryCamera);
		OnUpdateCore(timestep);
	}

	void Scene::OnPlayStart()
	{
		m_Box2DPhysicsWorld = new b2World({ 0.0f, -9.8f });

		// Store original transforms
		{
			auto view = m_Registry.view<TransformComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				TransformComponent& transformComponent = entity.GetComponent<TransformComponent>();
				transformComponent.TempTranslation = transformComponent.Translation;
				transformComponent.TempRotation = transformComponent.Rotation;
				transformComponent.TempScale = transformComponent.Scale;
			}
		}


		//auto view = m_Registry.view<TransformComponent, Box2DRigidbodyComponent>();
		auto view = m_Registry.view<Box2DRigidbodyComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			TransformComponent& transformComponent = entity.GetComponent<TransformComponent>();
			Box2DRigidbodyComponent& rigidBodyComponent = entity.GetComponent<Box2DRigidbodyComponent>();

			// Def
			b2BodyDef bodyDef;
			bodyDef.type = rigidBodyComponent.BodyType;
			bodyDef.position.Set(transformComponent.Translation.x, transformComponent.Translation.y);
			bodyDef.angle = transformComponent.Rotation.z;

			// Body
			rigidBodyComponent.RuntimeBody = m_Box2DPhysicsWorld->CreateBody(&bodyDef);
			rigidBodyComponent.RuntimeBody->SetFixedRotation(rigidBodyComponent.FixedRotation);

			if (entity.HasComponent<Box2DBoxColliderComponent>())
			{
				Box2DBoxColliderComponent& boxColliderComponent = entity.GetComponent<Box2DBoxColliderComponent>();

				// Box Shape
				b2PolygonShape boxShape;
				boxShape.SetAsBox(boxColliderComponent.Size.x * transformComponent.Scale.x, boxColliderComponent.Size.y * transformComponent.Scale.y);

				// Fixture Def
				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = boxColliderComponent.Density;
				fixtureDef.friction = boxColliderComponent.Friction;
				fixtureDef.restitution = boxColliderComponent.Restitution;
				fixtureDef.restitutionThreshold = boxColliderComponent.RestitutionThreshold;

				// Body
				rigidBodyComponent.RuntimeBody->CreateFixture(&fixtureDef);
			}
		}
	}

	void Scene::OnPlayEnd()
	{
		// Restore original transforms
		{
			auto view = m_Registry.view<TransformComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				TransformComponent& transformComponent = entity.GetComponent<TransformComponent>();
				transformComponent.Translation = transformComponent.TempTranslation;
				transformComponent.TempRotation = transformComponent.TempRotation;
				transformComponent.Scale = transformComponent.TempScale;
			}
		}
		delete m_Box2DPhysicsWorld;
		m_Box2DPhysicsWorld = nullptr;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = Entity(m_Registry.create(), this);
		entity.AddComponent<NameComponent>(name);
		entity.AddComponent<TransformComponent>();
		return entity;

		//Entity entity = { m_Registry.create(), this };
		//entity.AddComponent<TransformComponent>();
		//auto& tag = entity.AddComponent<NameComponent>();
		//tag.Name = name.empty() ? "Entity" : name;
		//return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

    template<typename T>
    void Scene::OnComponentAdded(Entity entity, T& component)
    {
        static_assert(false);
    }

    template<>
    void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<NameComponent>(Entity entity, NameComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<QuadRendererComponent>(Entity entity, QuadRendererComponent& component)
    {
    }

	template<>
	void Scene::OnComponentAdded<TriangleRendererComponent>(Entity entity, TriangleRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<Box2DRigidbodyComponent>(Entity entity, Box2DRigidbodyComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<Box2DBoxColliderComponent>(Entity entity, Box2DBoxColliderComponent& component)
	{
	}
}