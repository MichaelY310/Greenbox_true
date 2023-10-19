#pragma once
#include <Core/Log.h>
#include <glad/glad.h>

namespace Greenbox {

	class Texture2D
	{
	public:
		Texture2D(const char* filePath);			// for pictures
		Texture2D(uint32_t plainColor, uint32_t width, uint32_t height);	// for not picture, pure color
		static std::shared_ptr<Texture2D> Create(uint32_t plainColor, uint32_t width, uint32_t height) { return std::make_shared<Texture2D>(plainColor, width, height); };
		static std::shared_ptr<Texture2D> Create(const char* filePath) { return std::make_shared<Texture2D>(filePath); };
		~Texture2D() { glDeleteTextures(1, &m_RendererID); }

		void Bind(uint32_t slot) const;
		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }
		uint32_t GetRendererID() { return m_RendererID; }
		uint32_t HasPicture() const { return m_IsPicture; }

		void SetName(std::string name) { m_Name = name; }
		std::string GetName() { return m_Name; }
		const char* GetPath() { return m_Path; }

	private:
		std::string m_Name = "Untitled";
		const char* m_Path = "";
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