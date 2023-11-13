#pragma once

#include "entt.hpp"
#include "Render/Camera.h"

namespace Greenbox {

	class Entity;

	class Scene
	{
	public:
		Scene() = default;
		Scene(uint32_t width, uint32_t height)
			:m_ViewportWidth(width), m_ViewportHeight(height), m_Registry() {}
		~Scene() = default;

		void OnUpdate(); // The shared part of Edit and Play
		void OnUpdateEdit(Camera& EditorCamera);
		void OnUpdatePlay();

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


		friend class Entity;
		friend class EntityInspectorPanel;
	};

}