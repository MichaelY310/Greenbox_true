#pragma once
#include "Greenbox.h"
#include <glad/glad.h>


namespace Greenbox {

	class RendererLayer : public Layer
	{
	public:
		RendererLayer();
		~RendererLayer() = default;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

	private:
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);

	private:
		std::string m_Name;
		Framebuffer m_Framebuffer;
		std::shared_ptr<Scene> m_ActiveScene;

		Entity squareEntity;
		Entity triangleEntity;

		Camera m_EditorCamera;
		std::shared_ptr<Texture2D> texture;
		std::shared_ptr<Texture2D> whiteTexture;
		std::shared_ptr<Texture2D> texture1;
		std::shared_ptr<Texture2D> texture2;
		std::shared_ptr<Texture2D> texture3;

	// viewport
	private:
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		uint32_t m_ViewportSizeInitialized = 0; // 0: uninitialized  1: Obtained size from ImGui  2: Initialized
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };		// slightly smaller than window because of the offset
		glm::vec2 m_ViewportOffset = { 0.0f, 0.0f };	// the offset from the corner of viewport to window
		glm::vec2 m_ViewportWindowSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportMinBound = { 0.0f, 0.0f };	// Coordinate of bottom left corner
		glm::vec2 m_ViewportMaxBound = { 0.0f, 0.0f };	// Coordinate of top right corner
		glm::vec2 m_ViewportMousePos = { 0.0f, 0.0f };	// Relative to MinBound

		Entity m_HoveredEntity;
	// panel
	private:
		EntityInspectorPanel m_EntityInspectorPanel;

	// ImGizmo
	private:
		int m_GuizmoType = -1;
		bool m_Snap = false;
		float m_SnapValue = 0.5f;
	};

}