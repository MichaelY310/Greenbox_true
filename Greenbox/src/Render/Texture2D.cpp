#include "gbpch.h"
#include "Texture2D.h"
#include "stb_image.h"

namespace Greenbox {

	Texture2D::Texture2D(const char* filePath)
		: m_Path(filePath), plainColor(0xffffffff), m_Width(0), m_Height(0), m_IsPicture(true)
	{
		stbi_set_flip_vertically_on_load(true);
		int width, height, n_Channels;
		stbi_uc* data = stbi_load(filePath, &width, &height, &n_Channels, 0);
		//GB_INFO(filePath);
		GB_ASSERT(data, "Failed to load Texture2D {0}", filePath);
		m_Width = width;
		m_Height = height;
		m_n_Channels = n_Channels;
		GB_INFO("Texture2D::Texture2D   there are {0} channels", n_Channels);
		if (n_Channels == 4)
		{
			m_InternalFormat = GL_RGBA8;
			m_DataFormat = GL_RGBA;
		} 
		else if (n_Channels == 3)
		{
			m_InternalFormat = GL_RGB8;
			m_DataFormat = GL_RGB;
		}
		else {
			GB_ASSERT(false, "Greenbox doesn't support {0} channels data format", n_Channels);
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	Texture2D::Texture2D(uint32_t plainColor, uint32_t width, uint32_t height)
		: plainColor(plainColor), m_Width(width), m_Height(height), m_InternalFormat(GL_RGBA8), m_DataFormat(GL_RGBA), m_IsPicture(false), m_n_Channels(0)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, &plainColor);
	}

	void Texture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

}