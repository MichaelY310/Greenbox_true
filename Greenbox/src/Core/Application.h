#pragma once
#include "Core/Log.h"
#include <memory>
#include "Window.h"
#include "Layer.h"
#include "ImGui/ImGuiLayer.h"

namespace Greenbox {

	class Application
	{
	public:
		Application(const std::string appName);
		virtual ~Application();
		static Application& GetInstance() { return *s_Instance; }

		Window& GetWindow() { return *m_Window; };
		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
		void Run();
		void Stop();

		// Application Event
		void OnEvent(Event& e);
		// event handlers
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		// Layers
		void PushLayer(Layer* layer) { m_LayerStack.PushLayer(layer); layer->OnAttach(); }
		void PushOverLay(Layer* layer) { m_LayerStack.PushOverLay(layer); layer->OnAttach(); }
		void PopLayer(Layer* layer) { m_LayerStack.PopLayer(layer); layer->OnAttach(); }
		void PopOverLay(Layer* layer) { m_LayerStack.PopOverLay(layer); layer->OnAttach(); }

	private:
		std::unique_ptr<Window> m_Window;

		std::string m_AppName;
		bool m_Running = true;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;		// this is defined in Application::Application
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
	};


	Application* CreateApplication();

}