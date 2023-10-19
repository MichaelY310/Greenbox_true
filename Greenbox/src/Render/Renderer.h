#pragma once
#include <glad/glad.h>
#include "Render/Shader.h"
#include "Render/Texture2D.h"
#include "Render/Camera.h"
#include "Render/DataTypes.h"
#include <unordered_map>

namespace Greenbox {

	class Renderer {
	public:
		Renderer();
		~Renderer() = default;

		void ClearScene();
		void SetupRenderState();
		void SetCamera(Camera camera);
		void Draw();
		void NewBatch();

		void AddQuad(glm::mat4 transform = glm::mat4(1.0f),
			glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
			std::string texturePath = "");

		void AddQuad(glm::mat4 transform = glm::mat4(1.0f),
			glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
			const std::shared_ptr<Texture2D>& texture = nullptr);

		void PrintTextureInRepo()
		{
			GB_INFO("///////////////////////////");
			for (uint32_t i = 0; i < TextureRepoIndex; i++)
			{
				GB_INFO(m_TextureRepoPaths[i]);
			}
			GB_INFO("///////////////////////////");
		}

	private:
		struct vertexData
		{
			glm::vec3 Position;
			glm::vec4 Color;
			glm::vec2 TexCoord;
			float TexSlot;
		};

		ShaderDataType DataTypes[4] = { ShaderDataType::Float3, ShaderDataType::Float4, ShaderDataType::Float2, ShaderDataType::Float };

		glm::vec4 vertexPositions[4] = {
			glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f),
			glm::vec4( 0.5f, -0.5f, 0.0f, 1.0f),
			glm::vec4( 0.5f,  0.5f, 0.0f, 1.0f),
			glm::vec4(-0.5f,  0.5f, 0.0f, 1.0f),
		};

		glm::vec2 textureCoords[4] = { 
			glm::vec2(0.0f, 0.0f), 
			glm::vec2(1.0f, 0.0f), 
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f)
		};

	private:
		uint32_t IndexCount = 0;
		uint32_t VertexCount = 0;
		uint32_t QuadCount = 0;
		uint32_t TextureSlotIndex = 1;
		uint32_t TextureRepoIndex = 0;

		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;
		static const uint32_t MaxTexturesRepo = 1024;

		uint32_t VAO;
		uint32_t VBO;
		uint32_t EBO;

		Shader m_Shader;

		std::array<std::shared_ptr<Texture2D>, MaxTextureSlots> m_TextureBatch;
		std::array<std::string, MaxTextureSlots> m_TextureBatchPaths;
		std::array<std::shared_ptr<Texture2D>, MaxTextureSlots> m_TextureRepo;
		std::array<std::string, MaxTexturesRepo> m_TextureRepoPaths;

		vertexData* vertexDataArray = nullptr;
		vertexData* VertexDataPtr = nullptr;

	private:
		int HasTextureInRepo(std::string path) 
		{
			for (uint32_t i = 0; i < TextureRepoIndex; i++)
			{
				if (path == m_TextureRepoPaths[i])
				{
					return i;
				}
			}
			return -1;
		}

		int HasTextureInBatch(std::string path)
		{
			for (uint32_t i = 0; i < TextureSlotIndex; i++)
			{
				if (path == m_TextureBatchPaths[i])
				{
					return i;
				}
			}
			return -1;
		}

		int HasTextureID(uint32_t RendererID)
		{
			for (uint32_t i = 1; i < TextureSlotIndex; i++)
			{
				if (m_TextureBatch[i].get()->GetRendererID() == RendererID)
				{
					return i;
				}
			}
			return -1;
		}
	};

}