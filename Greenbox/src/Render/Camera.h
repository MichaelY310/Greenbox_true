#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "Core/Log.h"
#include "Core/Event/MouseEvent.h"

namespace Greenbox {

	class Camera
	{
	public:
		Camera(glm::vec3 position, glm::vec3 orientation, float fov, float aspectRatio, float nearClip, float farClip);
		Camera();
		~Camera() = default;

		void OnUpdate();
		void OnEvent(Event& e);

		bool OnMouseScroll(MouseScrollEvent& e);

		void UpdateProjection();
		void UpdateView();

		const glm::mat4& GetProjection() const { return m_Projection; }
		const glm::mat4& GetView() const { return m_View; }
		glm::mat4 GetViewProjection() const { return m_Projection * m_View; }

		glm::quat GetOrientation() const;
		glm::vec3 Up() const;
		glm::vec3 Right() const;
		glm::vec3 Forward() const;

		void SetViewportSize(uint32_t width, uint32_t height) {
			m_ViewportWidth = width;
			m_ViewportHeight = height;
			UpdateProjection();
		};

	private:
		glm::mat4 m_Projection;
		glm::mat4 m_View;

		glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 1.0f);
		float m_Pitch = 0.0f, m_Yaw = 0.0f;	// -z

		uint32_t m_ViewportWidth = 1280;
		uint32_t m_ViewportHeight = 720;
		float m_FOV = 45.0f;
		float m_AspectRatio = 1.778f;
		float m_NearClip = 0.1f;
		float m_FarClip = 1000.0f;

		float m_MovementSpeed = 0.1f;
		float m_ZoomSpeed = 0.3f;
		float m_RotationSpeed = glm::radians(45.0f) * 0.1;

		glm::vec2 m_PreviousMousePosition;
	};

}