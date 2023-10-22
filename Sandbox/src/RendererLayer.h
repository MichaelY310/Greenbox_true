#pragma once
#include "Core/layer.h"
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
		virtual void OnEvent(Event& e) override;

	private:
		std::string m_Name;
		Shader m_FramebufferShader;
		Framebuffer m_Framebuffer;
		Scene m_ActiveScene;

		Entity squareEntity;

		Camera m_EditorCamera;
		std::shared_ptr<Texture2D> texture;
		std::shared_ptr<Texture2D> whiteTexture;
		std::shared_ptr<Texture2D> texture1;
		std::shared_ptr<Texture2D> texture2;
		std::shared_ptr<Texture2D> texture3;

	};

}