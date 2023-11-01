#include "RendererLayer.h"
#include "imgui/imgui.h"

namespace Greenbox {


	RendererLayer::RendererLayer()
		: Layer("RendererLayer"), 
		m_EditorCamera(),
		m_Framebuffer(TextureDataType::DEPTH24_STENCIL8, { { TextureDataType::RGBA8, TextureDataType::RGBA }, { TextureDataType::R32I, TextureDataType::RED_INTEGER } }),
		m_ActiveScene(std::make_shared<Scene>(1280, 720)),
		m_EntityInspectorPanel(m_ActiveScene)
	{
		GB_INFO("RendererLayer::RendererLayer");
	}

	void RendererLayer::OnAttach()
	{
		GB_INFO("RendererLayer::OnAttach   ", m_Name);

		Renderer::SetupRenderState();

		whiteTexture = Texture2D::Create(0xffffffff, 1, 1);
		texture1 = Texture2D::Create("assets/imgs/xiaobei.jpg");
		texture2 = Texture2D::Create("assets/imgs/quannai.jpg");
		texture3 = Texture2D::Create("assets/imgs/yan.jpg");

		squareEntity = m_ActiveScene->CreateEntity("square");
		//squareEntity.AddComponent<SpriteRendererComponent>(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

		triangleEntity = m_ActiveScene->CreateEntity("triangle");
		triangleEntity.AddComponent<TriangleRendererComponent>(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
	}

	void RendererLayer::OnDetach()
	{
		GB_INFO("RendererLayer::OnDetach   ", m_Name);

	}

	void RendererLayer::OnUpdate()
	{
		// Set Viewport size
		// Viewport is initially 0.0f. So before getting the size from ImGui, just use the default values.
		// Resize only if Framebuffer needs to be resized
		if ((m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (m_ViewportSize.x != m_Framebuffer.GetWidth() || m_ViewportSize.y != m_Framebuffer.GetHeight())) || m_ViewportSizeInitialized == 1)
		{
			m_ActiveScene->SetViewportSize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_Framebuffer.Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorCamera.SetViewportSize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_ViewportSizeInitialized = 2;
			//GB_INFO("Viewport Resized");
		}

		m_Framebuffer.Bind();

		m_EditorCamera.OnUpdate();

		Renderer::ClearScene();
		Renderer::SetCamera(m_EditorCamera);

		m_ActiveScene->OnUpdate();
		Renderer::AddTriangle(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f)), glm::vec4(0.5f, 1.0f, 1.0f, 1.0f), texture1);
		Renderer::AddQuad(glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec4(0.5f, 1.0f, 1.0f, 1.0f), texture2);
		Renderer::AddQuad(glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f)), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), texture3);
		
		Renderer::Draw();

		m_Framebuffer.Unbind();
	}

	void RendererLayer::OnImGuiRender()
	{
		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = minWinSizeX;


		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// New      create new scene
				if (ImGui::MenuItem("New", "Ctrl+N"))
					//NewScene();
				// Open     create new scene and deserialize scene file
				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					//OpenScene();
				// Save     serialize scene file
				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					//SaveSceneAs();
				// Exit
				if (ImGui::MenuItem("Exit")) 
					Application::GetInstance().Stop();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}


		m_EntityInspectorPanel.OnImGuiRender();

		ImGui::Begin("Settings");
		ImGui::Text("QuadIndexCount: %d", Renderer::GetQuadIndexCount());
		ImGui::Text("TriangleIndexCount: %d", Renderer::GetTriangleIndexCount());
		ImGui::End();

		ImGui::Begin("Viewport");
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		// Update viewport size
		m_ViewportSize = { viewportSize.x, viewportSize.y };
		m_ViewportSizeInitialized = m_ViewportSizeInitialized == 0 ? 1 : m_ViewportSizeInitialized;
		ImGui::Image((void*)(m_Framebuffer.GetColorAttachment(0)), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
		ImGui::End();

		ImGui::End();
	}

	void RendererLayer::OnEvent(Event& e)
	{
		//GB_INFO("RendererLayer::OnEvent   ", m_Name);
		m_EditorCamera.OnEvent(e);
	}

}