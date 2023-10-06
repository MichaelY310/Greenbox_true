#include "gbpch.h"
#include "Window.h"
#include "Core/Log.h"


namespace Greenbox {

	Window::Window(std::string title, uint32_t width, uint32_t height)
		: m_Title(title), m_Width(width), m_Height(height)
	{

		GB_INFO("Window::Window   Window is created");
		// Initialzation
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		m_GLFWwindow = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);
		if (m_GLFWwindow == NULL)
		{
			glfwTerminate();
			GB_ASSERT(false, "Failed to create GLFW window");
			return;
		}
		glfwMakeContextCurrent(m_GLFWwindow);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			GB_ASSERT(false, "Failed to initialize GLAD");
			return;
		}

		// Set WindowData
		m_WindowData.Title = title;
		m_WindowData.Width = width;
		m_WindowData.Height = height;
		glfwSetWindowUserPointer(m_GLFWwindow, &m_WindowData);

		// Set CallbackFunctions
		glfwSetWindowCloseCallback(m_GLFWwindow, [](GLFWwindow* window)
			{
				WindowData& currentWindowData = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event = WindowCloseEvent();
				currentWindowData.EventCallbackFn(event);
			});

		glfwSetWindowSizeCallback(m_GLFWwindow, [](GLFWwindow* window, int width, int height)
			{
				WindowData& currentWindowData = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowResizeEvent event = WindowResizeEvent(width, height);
				currentWindowData.EventCallbackFn(event);
			});

		glfwSetScrollCallback(m_GLFWwindow, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& currentWindowData = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseScrollEvent event = MouseScrollEvent((float)xOffset, (float)yOffset);
				currentWindowData.EventCallbackFn(event);
			});
	}

	Window::~Window()
	{
		GB_INFO("Window::~Window   Window is destroyed");

		glfwDestroyWindow(m_GLFWwindow);
		glfwTerminate();
	}

	void Window::OnUpdate() 
	{ 
		glfwSwapBuffers(m_GLFWwindow); 
		glfwPollEvents(); 
	}

}