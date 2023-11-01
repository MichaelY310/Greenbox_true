#include "gbpch.h"
#include "Framebuffer.h"

namespace Greenbox {

	Framebuffer::Framebuffer(TextureDataType depthAttachmentType,
		std::vector<std::pair<TextureDataType, TextureDataType>> colorAttachmentTypes,
		uint32_t width, uint32_t height)
		: m_DepthAttachmentType(depthAttachmentType), m_ColorAttachmentTypes(colorAttachmentTypes),
		m_Width(width), m_Height(height)
	{
		Initialize();
	}

	void Framebuffer::Initialize()
	{
		// Replace the original attachments so that the width and height of storage changes
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);

			m_ColorAttachments.clear();
			m_DepthAttachment = 0;
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);


		if (m_DepthAttachmentType != TextureDataType::None)
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);

			glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
			glTexStorage2D(GL_TEXTURE_2D, 1, OpenGLDataType(m_DepthAttachmentType), m_Width, m_Height);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

		}

		// Color
		int colorSize = m_ColorAttachmentTypes.size();
		if (colorSize != 0)
		{
			m_ColorAttachments.resize(colorSize);
			glCreateTextures(GL_TEXTURE_2D, colorSize, m_ColorAttachments.data());
			for (int i=0; i < colorSize; i++)
			{
				std::pair<TextureDataType, TextureDataType> dataType = m_ColorAttachmentTypes[i];
				GLenum internalFormat = OpenGLDataType(dataType.first);
				GLenum dataFormat = OpenGLDataType(dataType.second);

				glBindTexture(GL_TEXTURE_2D, m_ColorAttachments[i]);
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, dataFormat, GL_UNSIGNED_BYTE, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, m_ColorAttachments[i], 0);
			}
		}

		if (m_ColorAttachments.size() > 1)
		{
			GB_ASSERT(m_ColorAttachments.size() <= 4, "only supports 4 color attachments");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColorAttachments.size(), buffers);
		}
	}

	Framebuffer::~Framebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_DepthAttachment);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());

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
		Initialize();
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
			OpenGLDataType(m_ColorAttachmentTypes[colorAttachmentIndex].first), GL_INT, &value);
	}

}