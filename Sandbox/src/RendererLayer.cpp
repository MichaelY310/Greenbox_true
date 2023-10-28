#include "RendererLayer.h"
#include "imgui/imgui.h"

namespace Greenbox {


	RendererLayer::RendererLayer()
		: Layer("RendererLayer"), 
		m_EditorCamera(),
		m_FramebufferShader("assets/shaders/framebufferShader.vert", "assets/shaders/framebufferShader.frag"), 
		m_Framebuffer(TextureDataType::DEPTH24_STENCIL8, { { TextureDataType::RGBA8, TextureDataType::RGBA }, { TextureDataType::R32I, TextureDataType::RED_INTEGER } }),
		m_ActiveScene(1280, 720)
	{
		GB_INFO("RendererLayer::RendererLayer");
	}

	void RendererLayer::OnAttach()
	{
		GB_INFO("RendererLayer::OnAttach   ", m_Name);

		glViewport(0, 0, 1280, 720);
		Renderer::SetupRenderState();

		whiteTexture = Texture2D::Create(0xffffffff, 1, 1);
		texture1 = Texture2D::Create("assets/imgs/xiaobei.jpg");
		texture2 = Texture2D::Create("assets/imgs/quannai.jpg");
		texture3 = Texture2D::Create("assets/imgs/yan.jpg");

		squareEntity = m_ActiveScene.CreateEntity("square");
		//squareEntity.AddComponent<SpriteRendererComponent>(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

		triangleEntity = m_ActiveScene.CreateEntity("triangle");
		triangleEntity.AddComponent<TriangleRendererComponent>(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
	}

	void RendererLayer::OnDetach()
	{
		GB_INFO("RendererLayer::OnDetach   ", m_Name);

	}

	void RendererLayer::OnUpdate()
	{
		m_Framebuffer.Bind();

		m_EditorCamera.OnUpdate();

		Renderer::ClearScene();
		Renderer::SetCamera(m_EditorCamera);

		m_ActiveScene.OnUpdate();
		//Renderer::AddQuad(glm::mat4(1.0f), glm::vec4(1.0f, 0.5f, 1.0f, 1.0f), texture1);
		Renderer::AddTriangle(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f)), glm::vec4(0.5f, 1.0f, 1.0f, 1.0f), texture1);
		Renderer::AddQuad(glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec4(0.5f, 1.0f, 1.0f, 1.0f), texture2);
		Renderer::AddQuad(glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f)), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), texture3);
		
		Renderer::Draw();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		Renderer::ClearScene();

		m_FramebufferShader.Bind();
		float quadVertices[] = {
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};
		unsigned int quadVAO, quadVBO;
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glBindVertexArray(quadVAO);
		glDisable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D, m_Framebuffer.GetColorAttachment(0));
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void RendererLayer::OnEvent(Event& e)
	{
		//GB_INFO("RendererLayer::OnEvent   ", m_Name);
		m_EditorCamera.OnEvent(e);
	}

}