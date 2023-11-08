#include "gbpch.h"
#include "FileBrowserPanel.h"


namespace Greenbox {

	FileBrowserPanel::FileBrowserPanel(std::string assetPath)
		: m_RootPath(assetPath), m_CurrentPath(m_RootPath)
	{
		m_DirectoryIconImage = Texture2D::Create("D:/Greenbox/Greenbox/src/Scene/Panels/DirectoryIcon.png");
		m_FileIconImage = Texture2D::Create("D:/Greenbox/Greenbox/src/Scene/Panels/FileIcon.png");
	}

	void FileBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("File Browser");

		// <-
		if (ImGui::Button("<-") && m_CurrentPath != m_RootPath)
			m_CurrentPath = m_CurrentPath.parent_path();

		// Icons
		ImGui::Columns((int)(ImGui::GetContentRegionAvail().x / (m_IconPadding + m_IconSize)), 0, false);
		for (auto& path : std::filesystem::directory_iterator(m_CurrentPath))
		{
			ImGui::ImageButton((ImTextureID)(path.is_directory() ? m_DirectoryIconImage : m_FileIconImage)->GetRendererID(), { m_IconSize, m_IconSize }, { 0, 1 }, { 1, 0 });

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (path.is_directory())
				{
					m_CurrentPath = path;
				}
			}

			ImGui::TextWrapped(std::filesystem::relative(path, m_CurrentPath).filename().string().c_str());
			ImGui::NextColumn();
		}

		ImGui::Columns(1);
		ImGui::End();
	}

}