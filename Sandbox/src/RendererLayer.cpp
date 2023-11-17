#include "RendererLayer.h"
#include "imgui/imgui.h"
#include "ImGuizmo.h"
#include <filesystem>

namespace Greenbox {


	RendererLayer::RendererLayer()
		: Layer("RendererLayer"),
		m_EditorCamera(),
		m_Framebuffer(TextureDataType::DEPTH24_STENCIL8, { { TextureDataType::RGBA8, TextureDataType::RGBA }, { TextureDataType::R32I, TextureDataType::RED_INTEGER } }),
		m_ActiveScene(std::make_shared<Scene>(1280, 720)),
		m_EntityInspectorPanel(m_ActiveScene),
		m_FileBrowserPanel("assets"),
		m_ControllerUIPanel(&m_EditorCamera, m_ActiveScene)
	{
		GB_INFO("RendererLayer::RendererLayer");
	}

	void RendererLayer::OnAttach()
	{
		GB_INFO("RendererLayer::OnAttach   ", m_Name);

		Renderer::SetupRenderState();

		cameraEntity = m_ActiveScene->CreateEntity("camera");
		cameraEntity.AddComponent<CameraComponent>();

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
		Renderer::ClearScene();
		m_Framebuffer.ClearColorAttachment(1, -1);

		if (m_ViewportFocused)
		{
			m_EditorCamera.OnUpdate();
		}
		m_ActiveScene->OnUpdate(m_EditorCamera);


		// Select Entity
		if (m_ActiveScene->getSceneState() == Scene::SceneState::Edit && m_ViewportMousePos.x >= 0 && m_ViewportMousePos.y >= 0 && m_ViewportMousePos.x < m_ViewportSize.x && m_ViewportMousePos.y < m_ViewportSize.y)
		{
			int pixelData = m_Framebuffer.ReadPixel(1, m_ViewportMousePos.x, m_ViewportMousePos.y);
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
		}
		else
		{
			m_HoveredEntity = Entity();
		}

		m_Framebuffer.Unbind();
	}

	void RendererLayer::OnImGuiRender()
	{
		// ImGui Setup
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
		// Docking
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


		// Menu
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

		// Panel
		m_EntityInspectorPanel.OnImGuiRender();
		m_FileBrowserPanel.OnImGuiRender();
		m_ControllerUIPanel.OnImGuiRender();

		// Viewport
			// Update viewport related variables
		ImGui::Begin("Viewport");
		m_ViewportSizeInitialized = m_ViewportSizeInitialized == 0 ? 1 : m_ViewportSizeInitialized;

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
			// Block event
		Application::GetInstance().GetImGuiLayer()->SetBlock(!(m_ViewportFocused && m_ViewportHovered));
		//m_EditorCamera.SetFreezed(!(m_ViewportFocused && m_ViewportHovered));

		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportSize.x, viewportSize.y };

		ImVec2 viewportOffset = ImGui::GetCursorPos();
		m_ViewportOffset = { viewportOffset.x, viewportOffset.y };

		ImVec2 viewportWindowSize = ImGui::GetWindowSize();
		m_ViewportWindowSize = { viewportWindowSize.x, viewportWindowSize.y };

		ImVec2 viewportWindowPos = ImGui::GetWindowPos();
		m_ViewportMinBound = { viewportWindowPos.x + m_ViewportOffset.x, viewportWindowPos.y + m_ViewportOffset.y };
		m_ViewportMaxBound = m_ViewportWindowSize + m_ViewportMinBound;

		ImVec2 mousePos = ImGui::GetMousePos();
		m_ViewportMousePos = { mousePos.x - m_ViewportMinBound.x, m_ViewportSize.y - mousePos.y + m_ViewportMinBound.y };

			// Display Framebuffer
		ImGui::Image((void*)(m_Framebuffer.GetColorAttachment(0)), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });





			// Select Entity
			// ImGizmo
		Entity selectedEntity = m_EntityInspectorPanel.GetSelectedEntity();
		if (selectedEntity && m_GuizmoType != -1 && m_ActiveScene->getSceneState() == Scene::SceneState::Edit)
		{
				// Set snap values
			m_Snap = Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL);
			m_SnapValue = m_GuizmoType == ImGuizmo::OPERATION::ROTATE ? 45.0f : 0.5f;
			float SnapValues[3] = { m_SnapValue, m_SnapValue, m_SnapValue };
				// Transform will be manipulated
			TransformComponent& transformComponent = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = transformComponent.GetTransform();
				// ImGuizmo Setup
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(m_ViewportMinBound.x, m_ViewportMinBound.y, m_ViewportSize.x, m_ViewportSize.y);
			ImGuizmo::Manipulate(glm::value_ptr(m_EditorCamera.GetView()), glm::value_ptr(m_EditorCamera.GetProjection()),
				(ImGuizmo::OPERATION)m_GuizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, m_Snap ? SnapValues : nullptr);
				// Update Manipulation
			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scaling;
				DecomposeTransform(transform, translation, rotation, scaling);
				transformComponent.Translation = translation;
				transformComponent.Rotation = rotation;
				transformComponent.Scale = scaling;
			}
		}

		ImGui::End();


		// Debugging Info
		ImGui::Begin("Window & Viewport info");
		ImGui::Text("ViewportFocused: %s", m_ViewportFocused ? "True" : "False");
		ImGui::Text("ViewportHovered: %s", m_ViewportHovered ? "True" : "False");
		ImGui::Text("ViewportSize: %f, %f", m_ViewportSize.x, m_ViewportSize.y);
		ImGui::Text("ViewportOffset: %f, %f", m_ViewportOffset.x, m_ViewportOffset.y);
		ImGui::Text("ViewportWindowSize: %f, %f", m_ViewportWindowSize.x, m_ViewportWindowSize.y);
		ImGui::Text("ViewportMinBound: %f, %f", m_ViewportMinBound.x, m_ViewportMinBound.y);
		ImGui::Text("Viewport MaxBound: %f, %f", m_ViewportMaxBound.x, m_ViewportMaxBound.y);
		ImGui::Text("Viewport Mouse Position: %f, %f", m_ViewportMousePos.x, m_ViewportMousePos.y);
		ImGui::Text("Hovered Entity: %s", bool(m_HoveredEntity) ? m_HoveredEntity.GetComponent<NameComponent>().Name.c_str() : "None");
		ImGui::End();

		// Render Info
		ImGui::Begin("Render Info");
		ImGui::Text("QuadIndexCount: %d", Renderer::GetQuadIndexCount());
		ImGui::Text("TriangleIndexCount: %d", Renderer::GetTriangleIndexCount());
		ImGui::End();

		ImGui::End();
	}

	void RendererLayer::OnEvent(Event& e)
	{
		//GB_INFO("RendererLayer::OnEvent   ", m_Name);
		m_EditorCamera.OnEvent(e);
		dispatcher.Dispatch<MouseButtonPressedEvent>(e, GB_BIND_FUNCTION(RendererLayer::OnMouseButtonPressed));
		dispatcher.Dispatch<KeyPressedEvent>(e, GB_BIND_FUNCTION(RendererLayer::OnKeyPressed));
	}

	bool RendererLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		//GB_INFO("RendererLayer::OnMouseButtonPressed {0}", e.GetButton());
		if (e.GetButton() == 0)
		{
			if (m_HoveredEntity)
				m_EntityInspectorPanel.SetSelectedEntity(m_HoveredEntity);
		}

		return false;
	}

	bool RendererLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		//GB_INFO("RendererLayer::OnMouseButtonPressed {0}", e.GetButton());
		if (e.GetRepeatCount() == 1)
			return false;

		int keyCode = e.GetKeyCode();

		switch (keyCode)
		{
			case GLFW_KEY_Z:
				m_GuizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case GLFW_KEY_X:
				m_GuizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			case GLFW_KEY_C:
				m_GuizmoType = ImGuizmo::OPERATION::SCALE;
				break;
			case GLFW_KEY_1:
				m_EditorCamera.SetMode(0);
				break;
			case GLFW_KEY_2:
				m_EditorCamera.SetMode(1);
				break;
			case GLFW_KEY_9:
				m_ActiveScene->SetSceneState(Scene::SceneState::Edit);
				break;
			case GLFW_KEY_0:
				m_ActiveScene->SetSceneState(Scene::SceneState::Play);
				break;
		}

		return false;
	}

}