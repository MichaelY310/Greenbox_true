#pragma once

#include "entt.hpp"
#include "Render/Camera.h"

class b2World;

namespace Greenbox {

	class Entity;

	class Scene
	{
	public:
		enum class SceneState
		{
			Edit = 0,
			Play = 1
		};
	public:
		Scene() = default;
		Scene(uint32_t width, uint32_t height)
			:m_ViewportWidth(width), m_ViewportHeight(height), m_Registry() {}
		~Scene() = default;

		void OnUpdate(Camera& EditorCamera);
		void OnUpdateCore(); // The shared part of Edit and Play
		void OnUpdateEdit(Camera& EditorCamera);
		void OnUpdatePlay();
		void OnPlayStart();
		void OnPlayEnd();
		void SetSceneState(SceneState sceneState) { m_SceneState = sceneState; };
		const SceneState getSceneState() const { return m_SceneState; };

		Entity CreateEntity(const std::string& name = "Untitled");
		void DestroyEntity(Entity entity);

		void SetViewportSize(uint32_t width, uint32_t height) {
			m_ViewportWidth = width; 
			m_ViewportHeight = height;
		};

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;

		uint32_t m_ViewportWidth = 1280;
		uint32_t m_ViewportHeight = 720;

		SceneState m_SceneState = SceneState::Edit;

		b2World* m_Box2DPhysicsWorld = nullptr;

		friend class Entity;
		friend class EntityInspectorPanel;
	};

}