#pragma once
#include "Core/Log.h"
#include "glad/glad.h"
#include "DataTypes.h"

namespace Greenbox {

	class Framebuffer
	{
	public:
		Framebuffer(std::vector<TextureDataType> depthAttachmentTypes, std::vector<TextureDataType> colorAttachmentTypes);
		~Framebuffer();

		void Bind();
		void Unbind();

		void Resize(uint32_t width, uint32_t height);
		int ReadPixel(uint32_t attachmentIndex, int x, int y);
		void ClearColorAttachment(uint32_t colorAttachmentIndex, int value);

	private:
		uint32_t m_Width = 1280;
		uint32_t m_Height = 720;

		uint32_t m_RendererID;
		std::vector<uint32_t> m_DepthAttachments;
		std::vector<uint32_t> m_ColorAttachments;

		std::vector<TextureDataType> m_DepthAttachmentTypes;
		std::vector<TextureDataType> m_ColorAttachmentTypes;
	};

}