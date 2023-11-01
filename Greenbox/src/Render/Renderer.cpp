#include "gbpch.h"
#include "Renderer.h"
#include "Render/Shader.h"

namespace Greenbox {

	struct quadVertexData
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexSlot;
		int EntityID;
	};

	struct triangleVertexData
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexSlot;
		int EntityID;
	};

	struct RendererData
	{
		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;


		// Quad
		ShaderDataType QuadDataTypes[5] = {
			ShaderDataType::Float3,
			ShaderDataType::Float4,
			ShaderDataType::Float2,
			ShaderDataType::Float,
			ShaderDataType::Int };
		uint32_t QuadIndexCount = 0;
		quadVertexData* QuadVertexDataBase = nullptr;
		quadVertexData* QuadVertexDataPtr = nullptr;
		Shader QuadShader;
		uint32_t QuadVAO;
		uint32_t QuadVBO;
		uint32_t QuadEBO;
		glm::vec4 QuadVertexPositions[4] = {
			glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f),
			glm::vec4(0.5f, -0.5f, 0.0f, 1.0f),
			glm::vec4(0.5f,  0.5f, 0.0f, 1.0f),
			glm::vec4(-0.5f,  0.5f, 0.0f, 1.0f)};
		glm::vec2 QuadTextureCoords[4] = {
			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f)};



		// Triangle
		ShaderDataType TriangleDataTypes[5] = {
			ShaderDataType::Float3,
			ShaderDataType::Float4,
			ShaderDataType::Float2,
			ShaderDataType::Float,
			ShaderDataType::Int };
		uint32_t TriangleIndexCount = 0;
		triangleVertexData* TriangleVertexDataBase = nullptr;
		triangleVertexData* TriangleVertexDataPtr = nullptr;
		Shader TriangleShader;
		uint32_t TriangleVAO;
		uint32_t TriangleVBO;
		uint32_t TriangleEBO;
		glm::vec4 TriangleVertexPositions[3] = {
			glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f),
			glm::vec4(0.5f, -0.5f, 0.0f, 1.0f),
			glm::vec4(0.0f,  0.5f, 0.0f, 1.0f) };
		glm::vec2 TriangleTextureCoords[3] = {
			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f) };


		uint32_t TextureSlotIndex = 1;
		std::array<std::shared_ptr<Texture2D>, MaxTextureSlots> m_TextureBatch;
		std::array<std::string, MaxTextureSlots> m_TextureBatchPaths;

	};

	static RendererData s_Data;

	void Renderer::Init()
	{
		// initialize textures
		s_Data.m_TextureBatchPaths[0] = "";
		s_Data.m_TextureBatch[0] = std::make_shared<Texture2D>(0xffffffff, 1, 1);
		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;


		// Quad
		s_Data.QuadShader = Shader("assets/shaders/quadShader.vert", "assets/shaders/quadShader.frag");
		s_Data.QuadShader.Bind();
		s_Data.QuadShader.setIntArray("u_Texture", samplers, s_Data.MaxTextureSlots);
		// VBO
		glCreateBuffers(1, &s_Data.QuadVBO);
		glBindBuffer(GL_ARRAY_BUFFER, s_Data.QuadVBO);
		glBufferData(GL_ARRAY_BUFFER, s_Data.MaxVertices * sizeof(quadVertexData), nullptr, GL_DYNAMIC_DRAW);// Initially empty. Filled in Draw().
		// VAO
		glCreateVertexArrays(1, &s_Data.QuadVAO);
		glBindVertexArray(s_Data.QuadVAO);
		SetVAO(s_Data.QuadVAO, s_Data.QuadDataTypes, (int)(sizeof(s_Data.QuadDataTypes) / sizeof(s_Data.QuadDataTypes[0])));
		// EBO
		uint32_t* QuadIndices = GenQuadIndices(s_Data.MaxIndices);
		glCreateBuffers(1, &s_Data.QuadEBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.QuadEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, s_Data.MaxIndices * sizeof(uint32_t), QuadIndices, GL_STATIC_DRAW);
		delete[] QuadIndices;
		// VertexDataBase
		s_Data.QuadVertexDataBase = new quadVertexData[s_Data.MaxVertices];



		// Triangle
		s_Data.TriangleShader = Shader("assets/shaders/triangleShader.vert", "assets/shaders/triangleShader.frag");
		s_Data.TriangleShader.Bind();
		s_Data.TriangleShader.setIntArray("u_Texture", samplers, s_Data.MaxTextureSlots);
		// VBO
		glCreateBuffers(1, &s_Data.TriangleVBO);
		glBindBuffer(GL_ARRAY_BUFFER, s_Data.TriangleVBO);
		glBufferData(GL_ARRAY_BUFFER, s_Data.MaxVertices * sizeof(triangleVertexData), nullptr, GL_DYNAMIC_DRAW);// Initially empty. Filled in Draw().
		// VAO
		glCreateVertexArrays(1, &s_Data.TriangleVAO);
		glBindVertexArray(s_Data.TriangleVAO);
		SetVAO(s_Data.TriangleVAO, s_Data.TriangleDataTypes, (int)(sizeof(s_Data.TriangleDataTypes) / sizeof(s_Data.TriangleDataTypes[0])));
		// EBO
		uint32_t* TriangleIndices = GenTriangleIndices(s_Data.MaxIndices);
		glCreateBuffers(1, &s_Data.TriangleEBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.TriangleEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, s_Data.MaxIndices * sizeof(uint32_t), TriangleIndices, GL_STATIC_DRAW);
		delete[] TriangleIndices;
		// VertexDataBase
		s_Data.TriangleVertexDataBase = new triangleVertexData[s_Data.MaxVertices];
	}

	void Renderer::Shutdown()
	{

	}

	void Renderer::ClearScene()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void Renderer::SetupRenderState()
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Renderer::Draw()
	{
		if (s_Data.QuadIndexCount)
		{
			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
				s_Data.m_TextureBatch[i]->Bind(i);

			// Fill VBO
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexDataPtr - (uint8_t*)s_Data.QuadVertexDataBase);
			glBindBuffer(GL_ARRAY_BUFFER, s_Data.QuadVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, s_Data.QuadVertexDataBase);

			// Draw
			s_Data.QuadShader.Bind();
			glBindVertexArray(s_Data.QuadVAO);
			glDrawElements(GL_TRIANGLES, s_Data.QuadIndexCount, GL_UNSIGNED_INT, nullptr);
			//glBindVertexArray(0);
		}

		if (s_Data.TriangleIndexCount)
		{
			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
				s_Data.m_TextureBatch[i]->Bind(i);

			// Fill VBO
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.TriangleVertexDataPtr - (uint8_t*)s_Data.TriangleVertexDataBase);
			glBindBuffer(GL_ARRAY_BUFFER, s_Data.TriangleVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, s_Data.TriangleVertexDataBase);

			// Draw
			s_Data.TriangleShader.Bind();
			glBindVertexArray(s_Data.TriangleVAO);
			glDrawElements(GL_TRIANGLES, s_Data.TriangleIndexCount, GL_UNSIGNED_INT, nullptr);
			//glBindVertexArray(0);
		}
	}

	void Renderer::NewBatch()
	{
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexDataPtr = s_Data.QuadVertexDataBase;

		s_Data.TriangleIndexCount = 0;
		s_Data.TriangleVertexDataPtr = s_Data.TriangleVertexDataBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer::SetCamera(Camera camera)
	{
		s_Data.QuadShader.Bind();
		s_Data.QuadShader.setMat4("u_ViewProjection", camera.GetViewProjection());
		s_Data.TriangleShader.Bind();
		s_Data.TriangleShader.setMat4("u_ViewProjection", camera.GetViewProjection());
		NewBatch();
	}

	void Renderer::AddQuad(const glm::mat4 transform, const glm::vec4 color, 
		const std::shared_ptr<Texture2D>& texture, const int entityID)
	{
		if (s_Data.QuadIndexCount+6 > s_Data.MaxIndices)
		{
			Draw();
			NewBatch();
		}

		uint32_t textureIndex;
		// case 1  white texture
		if (texture == nullptr)
		{
			textureIndex = 0;
		}
		else
		{
			int stored = HasTextureID(texture.get()->GetRendererID());
			// case 2  not stored
			if (stored == -1)
			{
				s_Data.m_TextureBatchPaths[s_Data.TextureSlotIndex] = texture->GetPath();
				s_Data.m_TextureBatch[s_Data.TextureSlotIndex] = texture;
				textureIndex = s_Data.TextureSlotIndex;
				s_Data.TextureSlotIndex += 1;
			}
			// case 3  stored
			else {
				textureIndex = stored;
			}
		}

		for (size_t i = 0; i < 4; i++)
		{
			s_Data.QuadVertexDataPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexDataPtr->Color = color;
			s_Data.QuadVertexDataPtr->TexCoord = s_Data.QuadTextureCoords[i];
			s_Data.QuadVertexDataPtr->TexSlot = textureIndex;
			s_Data.QuadVertexDataPtr->EntityID = entityID;
			s_Data.QuadVertexDataPtr++;
		}

		s_Data.QuadIndexCount += 6;
	}

	void Renderer::AddTriangle(const glm::mat4 transform, const glm::vec4 color,
			const std::shared_ptr<Texture2D>& texture, const int entityID)
	{
		if (s_Data.TriangleIndexCount + 3 > s_Data.MaxIndices)
		{
			Draw();
			NewBatch();
		}

		uint32_t textureIndex;
		// case 1  white texture
		if (texture == nullptr)
		{
			textureIndex = 0;
		}
		else
		{
			int stored = HasTextureID(texture.get()->GetRendererID());
			// case 2  not stored
			if (stored == -1)
			{
				s_Data.m_TextureBatchPaths[s_Data.TextureSlotIndex] = texture->GetPath();
				s_Data.m_TextureBatch[s_Data.TextureSlotIndex] = texture;
				textureIndex = s_Data.TextureSlotIndex;
				s_Data.TextureSlotIndex += 1;
			}
			// case 3  stored
			else {
				textureIndex = stored;
			}
		}

		for (size_t i = 0; i < 3; i++)
		{
			s_Data.TriangleVertexDataPtr->Position = transform * s_Data.TriangleVertexPositions[i];
			s_Data.TriangleVertexDataPtr->Color = color;
			s_Data.TriangleVertexDataPtr->TexCoord = s_Data.TriangleTextureCoords[i];
			s_Data.TriangleVertexDataPtr->TexSlot = textureIndex;
			s_Data.TriangleVertexDataPtr->EntityID = entityID;
			s_Data.TriangleVertexDataPtr++;
		}

		s_Data.TriangleIndexCount += 3;
	}


	void Renderer::PrintTextureInBatch()
	{
		GB_INFO("///////////////////////////");
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			GB_INFO(s_Data.m_TextureBatchPaths[i]);
		}
		GB_INFO("///////////////////////////");
	}

	uint32_t Renderer::GetQuadIndexCount()
	{
		return s_Data.QuadIndexCount;
	}

	uint32_t Renderer::GetTriangleIndexCount()
	{
		return s_Data.TriangleIndexCount;
	}


	int Renderer::HasTextureID(uint32_t RendererID)
	{
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (s_Data.m_TextureBatch[i].get()->GetRendererID() == RendererID)
			{
				return i;
			}
		}
		return -1;
	}
}