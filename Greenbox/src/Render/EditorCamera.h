#pragma once
#include "Camera.h"
#include <glm/glm.hpp>

#include "Core/Event/MouseEvent.h"
#include "Core/Event/KeyEvent.h"

namespace Greenbox {

	class EditorCamera : public Camera
	{
	public:
		EditorCamera();
		~EditorCamera() = default;

		void OnUpdate(float timestep);
		void OnEvent(Event& e);

		bool OnMouseScroll(MouseScrollEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);

	private:
		glm::vec2 m_PreviousMousePosition;

		float m_MovementSpeed = 1.0f;
		float m_ZoomSpeed = 3.0f;
		float m_RotationSpeed = glm::radians(45.0f) * 1.0f;
	};

}