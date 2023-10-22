#pragma once
#include "Core/Log.h"
#include "glad/glad.h"
#include "DataTypes.h"

namespace Greenbox {

	class Framebuffer
	{
	public:
		Framebuffer(TextureDataType depthAttachmentType, std::vector<std::pair<TextureDataType, TextureDataType>> colorAttachmentTypes, uint32_t width = 1280, uint32_t height = 720);
		~Framebuffer();

		void Bind();
		void Unbind();

		uint32_t GetColorAttachment(int id) { return m_ColorAttachments[id]; };
		uint32_t GetDepthAttachment(int id) { return m_DepthAttachment; };

		void Resize(uint32_t width, uint32_t height);
		int ReadPixel(uint32_t attachmentIndex, int x, int y);
		void ClearColorAttachment(uint32_t colorAttachmentIndex, int value);

	private:
		uint32_t m_RendererID;

		uint32_t m_Width = 1280;
		uint32_t m_Height = 720;

		uint32_t m_DepthAttachment;
		std::vector<uint32_t> m_ColorAttachments;

		TextureDataType m_DepthAttachmentType;
		std::vector<std::pair<TextureDataType, TextureDataType>> m_ColorAttachmentTypes;
	};

}