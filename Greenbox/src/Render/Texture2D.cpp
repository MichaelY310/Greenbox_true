#include "gbpch.h"
#include "Texture2D.h"
#include "stb_image.h"

namespace Greenbox {

	Texture2D::Texture2D(char* filePath)
		: m_Path(filePath), plainColor(0xffffffff), m_Width(0), m_Height(0), m_IsPicture(true)
	{
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_set_flip_vertically_on_load(true);
		int width, height, n_Channels;
		stbi_uc* data = stbi_load(filePath, &width, &height, &n_Channels, 0);
		GB_ASSERT(data, "Failed to load Texture2D {0}", filePath);
		m_Width = width;
		m_Height = height;
		m_n_Channels = n_Channels;
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
		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}

	Texture2D::Texture2D(uint32_t plainColor, uint32_t width, uint32_t height)
		: plainColor(plainColor), m_Width(width), m_Height(height), m_InternalFormat(GL_RGBA8), m_DataFormat(GL_RGBA), m_IsPicture(false)
	{
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, &plainColor);
	}

	void Texture2D::Bind(uint32_t slot) const
	{
		//glBindTextureUnit(slot, m_RendererID);
	}

}