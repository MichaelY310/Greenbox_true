#include "RendererLayer.h"

namespace Greenbox {


	RendererLayer::RendererLayer()
		: m_EditorCamera(),
		Layer("RendererLayer"),
		m_Shader("assets/shaders/shader.vert", "assets/shaders/shader.frag")
	{
		GB_INFO("RendererLayer::RendererLayer");
	}

	void RendererLayer::OnAttach()
	{
		GB_INFO("RendererLayer::OnAttach   ", m_Name);







		float vertices[] = {
			 0.5f,  0.5f,  0.0f,  1.0f,  1.0f,  1.0f,
			 0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  1.0f,
			-0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f
		};

		//float vertices[] = {
		//	 0.5f,  0.5f,  0.0f,  1.0f,  1.0f,  0.0f,
		//	 0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,
		//	-0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  0.0f,
		//	-0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f
		//};

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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0 * sizeof(float)));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

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
		int32_t samplers[32];
		for (uint32_t i = 0; i < 32; i++)
			samplers[i] = i;
		m_Shader.setIntArray("u_Texture", samplers, 32);
		m_Shader.setMat4("u_ViewProjection", m_EditorCamera.GetViewProjection());


		// white texture
		Texture2D whiteTexture = Texture2D(0xff00ffff, 1, 1);
		whiteTexture.Bind(0);
		Texture2D texture = Texture2D("assets/imgs/xiaobei.jpg");
		texture.Bind(1);

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