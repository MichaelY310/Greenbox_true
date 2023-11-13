#include "gbpch.h"
#include "Camera.h"
#include "Core/Log.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Greenbox {

	Camera::Camera(glm::vec3 position, glm::vec3 orientation, float fov, float aspectRatio, float nearClip, float farClip)
		: m_Position(position), 
		m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip),
		m_Mode(1)
	{
		UpdateProjection();
		UpdateView();
	}

	Camera::Camera()
	{
		UpdateProjection();
		UpdateView();
	}

	void Camera::OnUpdate()
	{
		if (m_Freezed)
			return;

		UpdateProjection();
		UpdateView();
	}

	void Camera::UpdateProjection()
	{
		// Perspective
		m_AspectRatio = (float)m_ViewportWidth / (float)m_ViewportHeight;
		m_PerspectiveProjection = glm::perspective(m_FOV, m_AspectRatio, m_NearClip, m_FarClip);

		// Orthographic
		float halfHeight = glm::tan(m_FOV / 2.0f);
		float halfWidth = halfHeight * m_AspectRatio;
		float left = -halfWidth;
		float right = halfWidth;
		float bottom = -halfHeight;
		float top = halfHeight;

		// Create the orthographic projection matrix
		m_OrthographicProjection = glm::ortho(left, right, bottom, top, m_NearClip, m_FarClip);
		//m_OrthographicProjection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 1000.0f);
	}

	void Camera::UpdateView()
	{
		m_View = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(GetOrientation());
		m_View = glm::inverse(m_View);
	}

	glm::quat Camera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}

	glm::vec3 Camera::Up() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 Camera::Right() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 Camera::Forward() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}
}