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
		uint32_t VAO;
		Shader m_Shader;

		Camera m_EditorCamera;
		Renderer m_Renderer;
		std::shared_ptr<Texture2D> texture;
		std::shared_ptr<Texture2D> whiteTexture;
		std::shared_ptr<Texture2D> texture1;
		std::shared_ptr<Texture2D> texture2;
		std::shared_ptr<Texture2D> texture3;

	};

}