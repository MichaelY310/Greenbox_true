#pragma once
#include "Render/Texture2D.h"

#include <filesystem>
#include <imgui.h>

namespace Greenbox {

	class FileBrowserPanel
	{
	public:
		FileBrowserPanel(std::string assetPath);

		void OnImGuiRender();

	private:
		std::shared_ptr<Texture2D> m_FileIconImage;
		std::shared_ptr<Texture2D> m_DirectoryIconImage;

		std::filesystem::path m_RootPath;
		std::filesystem::path m_CurrentPath;

		float m_IconSize = 128.0f;
		float m_IconPadding = 16.0f;
	};

}