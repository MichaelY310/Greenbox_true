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
		Texture2D texture;

		Camera m_EditorCamera;
	};

}