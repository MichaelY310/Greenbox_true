#pragma once

#include <imgui.h>
#include <imgui_internal.h>

#include "../Scene.h"
#include "Render/EditorCamera.h"
#include "Render/Texture2D.h"

namespace Greenbox {

	// Control over 
	// 1. EditorCamera
	//		1. Projection: Perspective / Orthographic
	// 2. Scene
	//		1. SceneState: Edit / Play
	// 3. 
	class ControllerUIPanel
	{
	public:
		ControllerUIPanel(EditorCamera* ApplicationEditorCamera, std::shared_ptr<Scene> ApplicationActiveScene);
		void OnImGuiRender();
	private:
		EditorCamera* m_ApplicationEditorCamera;
		std::shared_ptr<Scene> m_ApplicationActiveScene;
		std::shared_ptr<Texture2D> m_PlayButtonImage;
		std::shared_ptr<Texture2D> m_StopButtonImage;

		std::shared_ptr<Texture2D> m_CameraPerspectiveImage;
		std::shared_ptr<Texture2D> m_CameraOrthographicImage;
	};

}