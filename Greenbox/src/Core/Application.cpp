#include "gbpch.h"
#include "Application.h"
#include "Utils.h"
#include "Render/Renderer.h"

namespace Greenbox {

	Application* Application::s_Instance = nullptr;
	

	Application::Application(const std::string appName)
		: m_AppName(appName), m_Running(true)
	{
		s_Instance = this;

		m_Window = std::make_unique<Window>(appName, 1280, 720);
		m_Window->SetEventCallbackFn(GB_BIND_FUNCTION(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverLay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		GB_INFO("Application is destroyed");

	}

	void Application::Run()
	{
		while (m_Running)
		{
			float time = (float)glfwGetTime();
			float timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate(timestep);
			}

			m_ImGuiLayer->Begin();
			{
				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::Stop()
	{
		m_Running = false;
	}

	void Application::OnEvent(Event& e)
	{
		dispatcher.Dispatch<WindowCloseEvent>(e, GB_BIND_FUNCTION(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(e, GB_BIND_FUNCTION(Application::OnWindowResize));

		for (auto layer = m_LayerStack.end(); layer != m_LayerStack.begin(); )
		{
			(*--layer)->OnEvent(e);
			if (e.m_Handled) {
				break;
			}
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		GB_INFO("Application::OnWindowClose");
		m_Running = false;
		return false;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		GB_INFO("Application::OnWindowResize");
		Renderer::SetViewport(0, 0, e.GetWidth(), e.GetHeight());
		return false;
	}

}