#pragma once

#include <memory>
#include <functional>
#include "Event/ApplicationEvent.h"
#include "Event/MouseEvent.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Greenbox {

	class Window
	{
	public:
		Window(std::string title, uint32_t width, uint32_t height);
		~Window();
		GLFWwindow* GetGLFWwindow() { return m_GLFWwindow; };
		void OnUpdate();

		void SetEventCallbackFn(std::function<void(Event&)> fn) { m_WindowData.EventCallbackFn = fn; }

		std::string GetTitle() { return m_Title; }
	private:
		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			std::function<void(Event&)> EventCallbackFn;
		};
	private:
		std::string m_Title;
		uint32_t m_Width;
		uint32_t m_Height;

		// The EventCallbackFunction is Application::OnEvent

		GLFWwindow* m_GLFWwindow;
		WindowData m_WindowData;
	};

}