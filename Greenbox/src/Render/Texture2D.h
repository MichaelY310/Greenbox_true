#pragma once
#include <Core/Log.h>
#include <glad/glad.h>

namespace Greenbox {

	class Texture2D
	{
	public:
		Texture2D(char* filePath);			// for pictures
		Texture2D(uint32_t plainColor, uint32_t width, uint32_t height);	// for not picture, pure color

		void Bind(uint32_t slot) const;
		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }
		uint32_t HasPicture() const { return m_IsPicture; }

		void SetName(char* path) { m_Path = path; }
	private:
		std::string m_Name = "Untitled";
		char* m_Path;
		uint32_t plainColor;
		uint32_t m_RendererID;
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_n_Channels;
		bool m_IsPicture;
		GLenum m_InternalFormat;
		GLenum m_DataFormat;		// file format
	};

}