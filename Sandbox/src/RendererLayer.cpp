#include "RendererLayer.h"

namespace Greenbox {


	RendererLayer::RendererLayer()
		: Layer("RendererLayer"), 
		m_EditorCamera(),
		m_Renderer(),
		m_Shader("assets/shaders/framebufferShader.vert", "assets/shaders/framebufferShader.frag")
	{
		GB_INFO("RendererLayer::RendererLayer");
	}

	void RendererLayer::OnAttach()
	{
		GB_INFO("RendererLayer::OnAttach   ", m_Name);

		glViewport(0, 0, 1280, 720);
		m_Renderer.SetupRenderState();

		whiteTexture = Texture2D::Create(0xffffffff, 1, 1);
		texture1 = Texture2D::Create("assets/imgs/xiaobei.jpg");
		texture2 = Texture2D::Create("assets/imgs/quannai.jpg");
		texture3 = Texture2D::Create("assets/imgs/yan.jpg");

	}

	void RendererLayer::OnDetach()
	{
		GB_INFO("RendererLayer::OnDetach   ", m_Name);

	}

	void RendererLayer::OnUpdate()
	{
		uint32_t FBO;
		glCreateFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		std::vector<uint32_t> m_ColorAttachments;
		m_ColorAttachments.resize(2);
		glCreateTextures(GL_TEXTURE_2D, 2, m_ColorAttachments.data());

		glBindTexture(GL_TEXTURE_2D, m_ColorAttachments[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1280, 720, 0, GL_RGBA8, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachments[0], 0);

		glBindTexture(GL_TEXTURE_2D, m_ColorAttachments[1]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, 1280, 720, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 1, GL_TEXTURE_2D, m_ColorAttachments[1], 0);

		uint32_t DepthTexture;
		glCreateTextures(GL_TEXTURE_2D, 1, &DepthTexture);
		glBindTexture(GL_TEXTURE_2D, DepthTexture);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, 1280, 720);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, DepthTexture, 0);

		GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(2, buffers);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			GB_ASSERT(false, "ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glViewport(0, 0, 1280, 720);





		m_EditorCamera.OnUpdate();

		m_Renderer.ClearScene();
		m_Renderer.SetCamera(m_EditorCamera);

		m_Renderer.AddQuad(glm::mat4(1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), "assets/imgs/shi.jpg");

		//m_Renderer.AddQuad(glm::mat4(1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), "assets/imgs/shi.jpg");
		//m_Renderer.AddQuad(glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), "assets/imgs/yan.jpg");
		//m_Renderer.AddQuad(glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f)), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), "assets/imgs/yan.jpg");
		//m_Renderer.AddQuad(glm::mat4(1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), texture1);
		//m_Renderer.AddQuad(glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), texture2);
		//m_Renderer.AddQuad(glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f)), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), texture3);

		
		m_Renderer.Draw();





		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		m_Renderer.ClearScene();

		m_Shader.Bind();
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
		glBindTexture(GL_TEXTURE_2D, m_ColorAttachments[0]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void RendererLayer::OnEvent(Event& e)
	{
		//GB_INFO("RendererLayer::OnEvent   ", m_Name);
		m_EditorCamera.OnEvent(e);
	}

}