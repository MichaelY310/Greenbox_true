#include "RendererLayer.h"

namespace Greenbox {


	RendererLayer::RendererLayer()
		: m_EditorCamera(Camera()),
		Layer("RendererLayer"),
		m_Shader(Shader("assets/shaders/shader.vert", "assets/shaders/shader.frag")),
		texture(Texture2D("assets/imgs/xiaobei.jpg"))
	{
		GB_INFO("RendererLayer::RendererLayer");
	}

	void RendererLayer::OnAttach()
	{
		GB_INFO("RendererLayer::OnAttach   ", m_Name);

		float vertices[] = {
			 0.5f,  0.5f,  0.0f,  1.0f,  1.0f,
			 0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f, -0.5f,  0.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.0f,  0.0f,  1.0f
		};
		unsigned int indices[] = {
			0, 1, 3,
			1, 2, 3
		};

		glViewport(0, 0, 1280, 720);
		glEnable(GL_DEPTH_TEST);

		uint32_t VBO, EBO;

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}

	void RendererLayer::OnDetach()
	{
		GB_INFO("RendererLayer::OnDetach   ", m_Name);

	}

	void RendererLayer::OnUpdate()
	{
		m_EditorCamera.OnUpdate();

		m_Shader.Bind();
		texture.Bind(0);
		m_Shader.setMat4("u_ViewProjection", m_EditorCamera.GetViewProjection());

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void RendererLayer::OnEvent(Event& e)
	{
		//GB_INFO("RendererLayer::OnEvent   ", m_Name);
		m_EditorCamera.OnEvent(e);
	}

}