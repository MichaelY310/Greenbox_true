#include "gbpch.h"
#include "Framebuffer.h"

namespace Greenbox {

	Framebuffer::Framebuffer(std::vector<TextureDataType> depthAttachmentTypes, std::vector<TextureDataType> colorAttachmentTypes)
		: m_DepthAttachmentTypes(depthAttachmentTypes), m_ColorAttachmentTypes(colorAttachmentTypes)
	{
		int depthSize = m_DepthAttachmentTypes.size();
		if (depthSize != 0)
		{
			m_ColorAttachments.resize(depthSize);
			glCreateTextures(GL_TEXTURE_2D, depthSize, m_ColorAttachments.data());
		}
		for (int i=0; i<depthSize; i++)
		{
			glBindTexture(GL_TEXTURE_2D, m_ColorAttachments[i]);
			if (colorAttachmentTypes[i] == TextureDataType::RGBA8)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			else if (colorAttachmentTypes[i] == TextureDataType::RGBA8)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, m_ColorAttachments[i], 0);

		}


		int colorSize = m_ColorAttachmentTypes.size();
		if (colorSize != 0)
		{

		}
		for (TextureDataType dataType : m_ColorAttachmentTypes)
		{

		}
	}

	Framebuffer::~Framebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(m_DepthAttachments.size(), m_DepthAttachments.data());
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());

		m_DepthAttachments.clear();
		m_ColorAttachments.clear();
	}

	void Framebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Width, m_Height);
	}

	void Framebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::Resize(uint32_t width, uint32_t height)
	{
		m_Width = width;
		m_Height = height;
	}

	int Framebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		GB_ASSERT(attachmentIndex < m_ColorAttachments.size(), "out of range");
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int value;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &value);
		return value;
	}

	void Framebuffer::ClearColorAttachment(uint32_t colorAttachmentIndex, int value)
	{
		GB_ASSERT(colorAttachmentIndex < m_ColorAttachments.size(), "out of range");
		glClearTexImage(m_ColorAttachments[colorAttachmentIndex], 0,
			OpenGLDataType(m_ColorAttachmentTypes[colorAttachmentIndex]), GL_INT, &value);
	}

}