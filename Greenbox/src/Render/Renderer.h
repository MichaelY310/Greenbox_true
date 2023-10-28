#pragma once
#include <glad/glad.h>
#include "Texture2D.h"
#include "Camera.h"
#include "DataTypes.h"
#include <unordered_map>

namespace Greenbox {

	class Renderer {
	public:
		static void Init();
		static void Shutdown();

		static void ClearScene();
		static void SetupRenderState();
		static void SetCamera(Camera camera);
		static void Draw();
		static void NewBatch();

		static void AddQuad(const glm::mat4 transform = glm::mat4(1.0f),
			const glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
			const std::shared_ptr<Texture2D>& texture = nullptr,
			const int entityID = -1);
		static void AddTriangle(const glm::mat4 transform = glm::mat4(1.0f),
			const glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
			const std::shared_ptr<Texture2D>& texture = nullptr,
			const int entityID = -1);

		static void PrintTextureInBatch();

	private:
		static int HasTextureID(uint32_t RendererID);
		static void SetVAO(uint32_t& VAO, ShaderDataType DataTypes[], int typeCount) 
		{
			glBindVertexArray(VAO);
			uint32_t stride = 0;
			for (int i = 0; i < typeCount; i++)
			{
				stride += ShaderDataTypeSize(DataTypes[i]);
			}
			uint32_t offset = 0;
			for (int i = 0; i < typeCount; i++)
			{
				glEnableVertexAttribArray(i);
				glVertexAttribPointer(i, GetComponentCount(DataTypes[i]), GL_FLOAT, GL_FALSE, stride, (void*)(offset));
				offset += ShaderDataTypeSize(DataTypes[i]);
			}
		};
		static uint32_t* GenQuadIndices(int indicesCount)
		{
			uint32_t offset = 0;
			uint32_t* indices = new uint32_t[indicesCount];
			for (uint32_t i = 0; i < indicesCount; i += 6)
			{
				indices[i + 0] = offset + 0;
				indices[i + 1] = offset + 1;
				indices[i + 2] = offset + 2;
				indices[i + 3] = offset + 2;
				indices[i + 4] = offset + 3;
				indices[i + 5] = offset + 0;
				offset += 4;
			}
			return indices;
		}

		static uint32_t* GenTriangleIndices(int indicesCount)
		{
			uint32_t offset = 0;
			uint32_t* indices = new uint32_t[indicesCount];
			for (uint32_t i = 0; i < indicesCount; i += 3)
			{
				indices[i + 0] = offset + 0;
				indices[i + 1] = offset + 1;
				indices[i + 2] = offset + 2;
				offset += 3;
			}
			return indices;
		}
	};
}