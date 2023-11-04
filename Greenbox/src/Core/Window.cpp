#include "gbpch.h"
#include "Window.h"
#include "Core/Log.h"


namespace Greenbox {

	static bool s_GLFWInitialized = false;

	Window::Window(std::string title, uint32_t width, uint32_t height)
		: m_Title(title), m_Width(width), m_Height(height)
	{
		GB_INFO("Window::Window   Window is created");


		// Initialze GLFW
		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			GB_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		// Initialize GLFWwindow
		m_GLFWwindow = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);
		if (m_GLFWwindow == NULL)
		{
			glfwTerminate();
			GB_ASSERT(false, "Failed to create GLFW window");
			return;
		}
		glfwMakeContextCurrent(m_GLFWwindow);

		// Initialize GLAD
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

		glfwSetMouseButtonCallback(m_GLFWwindow, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& currentWindowData = *(WindowData*)glfwGetWindowUserPointer(window);
				switch (action)
				{
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent event(button);
						currentWindowData.EventCallbackFn(event);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent event(button);
						currentWindowData.EventCallbackFn(event);
						break;
					}
				}
			});

		glfwSetKeyCallback(m_GLFWwindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				switch (action)
				{
					case GLFW_PRESS:
					{
						KeyPressedEvent event(key, 0);
						data.EventCallbackFn(event);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent event(key);
						data.EventCallbackFn(event);
						break;
					}
					case GLFW_REPEAT:
					{
						KeyPressedEvent event(key, 1);
						data.EventCallbackFn(event);
						break;
					}
				}
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
		glfwPollEvents();
		glfwSwapBuffers(m_GLFWwindow);
	}

}