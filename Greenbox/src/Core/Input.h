#pragma once
#include <memory>
#include <GLFW/glfw3.h>
#include "Core/Application.h"

namespace Greenbox {

	class Input
	{
	public:
        static bool IsKeyPressed(int keycode)
        {
            GLFWwindow* window = Application::GetInstance().GetWindow().GetGLFWwindow();
            GLenum state = glfwGetKey(window, keycode);
            return state == GLFW_PRESS || state == GLFW_REPEAT;
        }

        static bool IsMouseButtonPressed(int button)
        {
            GLFWwindow* window = Application::GetInstance().GetWindow().GetGLFWwindow();
            GLenum state = glfwGetMouseButton(window, button);
            return state == GLFW_PRESS;
        }

        static std::pair<float, float> GetMousePosition()
        {
            GLFWwindow* window = Application::GetInstance().GetWindow().GetGLFWwindow();
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            return { (float)xpos, (float)ypos };
        }

        static float GetMouseX()
        {
            auto [x, y] = GetMousePosition();
            return x;
        }

        static float GetMouseY()
        {
            auto [x, y] = GetMousePosition();
            return y;
        }
	};

}