#include "gbpch.h"
#include "EditorCamera.h"
#include "Core/Utils.h"
#include "Core/Input.h"

namespace Greenbox {

	EditorCamera::EditorCamera()
		: Camera()
	{
		m_PreviousMousePosition = { Input::GetMouseX(), Input::GetMouseY() };
	}

	void EditorCamera::OnUpdate()
	{
		if (m_Freezed)
			return;


		if (Input::IsKeyPressed(GLFW_KEY_Q))
		{
			m_Position += m_MovementSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
		}
		else if (Input::IsKeyPressed(GLFW_KEY_E))
		{
			m_Position += -m_MovementSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
		}
		else if (Input::IsKeyPressed(GLFW_KEY_W))
		{
			glm::vec3 FootDirection = glm::cross(Right(), glm::vec3(0.0, 1.0, 0.0));
			m_Position += -FootDirection * m_MovementSpeed;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_S))
		{
			glm::vec3 FootDirection = glm::cross(Right(), glm::vec3(0.0, 1.0, 0.0));
			m_Position += FootDirection * m_MovementSpeed;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_A))
		{
			m_Position += -Right() * m_MovementSpeed;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_D))
		{
			m_Position += Right() * m_MovementSpeed;
		}

		if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE))
		{
			float deltaX = Input::GetMouseX() - m_PreviousMousePosition.x;
			float deltaY = Input::GetMouseY() - m_PreviousMousePosition.y;
			if (Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
			{
				m_Position -= Right() * deltaX * m_MovementSpeed * 0.01f;
				m_Position += Up() * deltaY * m_MovementSpeed * 0.01f;
			}
			else {
				float yawSign = Up().y < 0 ? -1.0f : 1.0f;
				m_Yaw += yawSign * deltaX * m_RotationSpeed * 0.1;
				m_Pitch += deltaY * m_RotationSpeed * 0.1;
			}
		}
		m_PreviousMousePosition = glm::vec2(Input::GetMouseX(), Input::GetMouseY());
		Camera::OnUpdate();
	}

	void EditorCamera::OnEvent(Event& e)
	{
		dispatcher.Dispatch<MouseScrollEvent>(e, GB_BIND_FUNCTION(EditorCamera::OnMouseScroll));
	}

	bool EditorCamera::OnMouseScroll(MouseScrollEvent& e)
	{
		float delta = e.GetY() * m_ZoomSpeed;
		//m_Position += Forward() * delta * m_MovementSpeed;
		m_FOV -= delta * m_MovementSpeed;
		return true;
	}
}