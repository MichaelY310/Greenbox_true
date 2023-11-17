#include "gbpch.h"
#include "ControllerUIPanel.h"

namespace Greenbox {

	ControllerUIPanel::ControllerUIPanel(EditorCamera* ApplicationEditorCamera, std::shared_ptr<Scene> ApplicationActiveScene)
	{
		m_ApplicationEditorCamera = ApplicationEditorCamera;
		m_ApplicationActiveScene = ApplicationActiveScene;
		m_PlayButtonImage = Texture2D::Create("D:/Greenbox/Greenbox/src/Scene/Panels/PlayButton.png");
		m_StopButtonImage = Texture2D::Create("D:/Greenbox/Greenbox/src/Scene/Panels/StopButton.png");

		m_CameraPerspectiveImage = Texture2D::Create("D:/Greenbox/Greenbox/src/Scene/Panels/PlayButton.png");
		m_CameraOrthographicImage = Texture2D::Create("D:/Greenbox/Greenbox/src/Scene/Panels/StopButton.png");
	}

	void ControllerUIPanel::OnImGuiRender()
	{
		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		float AvailHeight = ImGui::GetWindowContentRegionMax().y - 25.0f;
		float AvailWidth = ImGui::GetWindowContentRegionMax().x;

		// PlayButton
		float size = AvailHeight - 4.0f;
		std::shared_ptr<Texture2D> icon = m_ApplicationActiveScene->getSceneState() == Scene::SceneState::Edit ? m_PlayButtonImage : m_StopButtonImage;
		// Upper Left
		ImGui::SetCursorPosX((AvailWidth * 0.5f) - (size * 0.5f));
		ImGui::SetCursorPosY(25.0f + (AvailHeight * 0.5f) - (size * 0.5f));
		if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0))
		{
			if (m_ApplicationActiveScene->getSceneState() == Scene::SceneState::Edit)
				m_ApplicationActiveScene->SetSceneState(Scene::SceneState::Play);
			else if (m_ApplicationActiveScene->getSceneState() == Scene::SceneState::Play)
				m_ApplicationActiveScene->SetSceneState(Scene::SceneState::Edit);
		}

		// Editor Camera Mode
		size = AvailHeight - 4.0f - 1.0f;
		std::shared_ptr<Texture2D> CameraModeIcon = m_ApplicationEditorCamera->GetMode() == 1 ? m_CameraPerspectiveImage : m_CameraOrthographicImage;
		ImGui::SetCursorPosX(AvailWidth - (size * 0.5f) - 10.0f);
		ImGui::SetCursorPosY(25.0f + (AvailHeight * 0.5f) - (size * 0.5f));
		if (ImGui::ImageButton((ImTextureID)CameraModeIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0))
		{
			if (m_ApplicationEditorCamera->GetMode() == 1)
				m_ApplicationEditorCamera->SetMode(0);
			else if (m_ApplicationEditorCamera->GetMode() == 0)
				m_ApplicationEditorCamera->SetMode(1);
		}

		// 

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}

}