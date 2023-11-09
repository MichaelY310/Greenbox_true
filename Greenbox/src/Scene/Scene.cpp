#include "gbpch.h"

#include "Scene.h"
#include "Render/Renderer.h"
#include "Components.h"
#include "Entity.h"

namespace Greenbox {

	void Scene::OnUpdate()
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
}