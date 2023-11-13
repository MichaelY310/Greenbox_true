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

		void OnUpdate();
		void OnEvent(Event& e);

		bool OnMouseScroll(MouseScrollEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);

	private:
		glm::vec2 m_PreviousMousePosition;
	};

}