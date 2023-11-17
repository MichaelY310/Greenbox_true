#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "Core/Log.h"

namespace Greenbox {

	class Camera
	{
		friend class EditorCamera;
	public:
		Camera(glm::vec3 position, glm::vec3 orientation, float fov, float aspectRatio, float nearClip, float farClip);
		Camera();
		~Camera() = default;

		void OnUpdate();

		void UpdateProjection();
		void UpdateView();

		const glm::mat4& GetProjection() const { return m_PerspectiveProjection; }
		const glm::mat4& GetView() const { return m_View; }
		const glm::mat4 GetViewProjection() const { return m_Mode == 0 ? m_OrthographicProjection * m_View : m_PerspectiveProjection * m_View; }
		const uint32_t GetFOV() const { return m_FOV; }
		const uint32_t GetNearClip() const { return m_NearClip; }
		const uint32_t GetFarClip() const { return m_FarClip; }
		const uint32_t GetMode() const { return m_Mode; }

		glm::quat GetOrientation() const;
		glm::vec3 Up() const;
		glm::vec3 Right() const;
		glm::vec3 Forward() const;

		void SetViewportSize(uint32_t width, uint32_t height) {
			m_ViewportWidth = width;
			m_ViewportHeight = height;
			UpdateProjection();
		};
		void SetFOV(uint32_t FOV) { m_FOV = FOV; }
		void SetNearClip(uint32_t nearClip) { m_NearClip = nearClip; }
		void SetFarClip(uint32_t farClip) { m_FarClip = farClip; }
		void SetMode(uint32_t mode) { m_Mode = mode; }
		void SetFreezed(bool freezed) { m_Freezed = freezed; }
		void SetView(glm::mat4 view) { m_View = view; }

	private:
		// 0: Orthographic
		// 1: Perspective
		uint32_t m_Mode = 1;

		glm::mat4 m_View;
		glm::mat4 m_PerspectiveProjection;
		glm::mat4 m_OrthographicProjection;

		glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 1.0f);
		float m_Pitch = 0.0f, m_Yaw = 0.0f;	// -z

		float m_FOV = 45.0f;
		float m_AspectRatio = 1.778f;
		float m_NearClip = 0.1f;
		float m_FarClip = 1000.0f;

	private:
		uint32_t m_ViewportWidth = 1280;
		uint32_t m_ViewportHeight = 720;

		float m_MovementSpeed = 0.1f;
		float m_ZoomSpeed = 0.3f;
		float m_RotationSpeed = glm::radians(45.0f) * 0.1;

		bool m_Freezed = false;
	};

}