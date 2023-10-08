#include "gbpch.h"
#include "Application.h"
#include "Utils.h"

namespace Greenbox {

	Application* Application::s_Instance = nullptr;
	

	Application::Application(const std::string appName)
		: m_AppName(appName), m_Running(true)
	{
		s_Instance = this;

		m_Window = std::make_unique<Window>(appName, 900, 600);
		m_Window->SetEventCallbackFn(GB_BIND_FUNCTION(Application::OnEvent));
	}

	Application::~Application()
	{
		GB_INFO("Application is destroyed");

	}

	void Application::Run()
	{
		while (m_Running)
		{
			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

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
		//glViewport(0, 0, e.GetWidth(), e.GetHeight());
		return false;
	}

}