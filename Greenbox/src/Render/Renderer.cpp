#include "gbpch.h"
#include "Renderer.h"
#include "Render/Shader.h"

namespace Greenbox {

	struct vertexData
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexSlot;
		int EntityID;
	};

	struct RendererData
	{
		ShaderDataType DataTypes[5] = {
			ShaderDataType::Float3,
			ShaderDataType::Float4,
			ShaderDataType::Float2,
			ShaderDataType::Float,
			ShaderDataType::Int };

		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		Shader m_Shader;
		uint32_t VAO;
		uint32_t VBO;
		uint32_t EBO;

		uint32_t IndexCount = 0;
		uint32_t VertexCount = 0;
		uint32_t QuadCount = 0;
		uint32_t TextureSlotIndex = 1;

		std::array<std::shared_ptr<Texture2D>, MaxTextureSlots> m_TextureBatch;
		std::array<std::string, MaxTextureSlots> m_TextureBatchPaths;

		vertexData* vertexDataArray = nullptr;
		vertexData* VertexDataPtr = nullptr;

		glm::vec4 vertexPositions[4] = {
			glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f),
			glm::vec4(0.5f, -0.5f, 0.0f, 1.0f),
			glm::vec4(0.5f,  0.5f, 0.0f, 1.0f),
			glm::vec4(-0.5f,  0.5f, 0.0f, 1.0f),
		};

		glm::vec2 textureCoords[4] = {
			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f)
		};
	};

	static RendererData s_Data;

	void Renderer::Init()
	{
		s_Data.vertexDataArray = new vertexData[s_Data.MaxVertices];

		// Texture
		//White Texture
		s_Data.m_TextureBatchPaths[0] = "";
		s_Data.m_TextureBatch[0] = std::make_shared<Texture2D>(0xffffffff, 1, 1);
		// initialize slots
		s_Data.m_Shader = Shader("assets/shaders/shader.vert", "assets/shaders/shader.frag");
		s_Data.m_Shader.Bind();
		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;
		s_Data.m_Shader.setIntArray("u_Texture", samplers, s_Data.MaxTextureSlots);



		///////////////////////////////////////////////////////
		// VBO
		glCreateBuffers(1, &s_Data.VBO);
		glBindBuffer(GL_ARRAY_BUFFER, s_Data.VBO);
		// Initially empty. Filled in Draw().
		glBufferData(GL_ARRAY_BUFFER, s_Data.MaxVertices * sizeof(vertexData), nullptr, GL_DYNAMIC_DRAW);

		// VAO
		glCreateVertexArrays(1, &s_Data.VAO);
		glBindVertexArray(s_Data.VAO);
		uint32_t stride = 0;
		for (int i = 0; i < sizeof(s_Data.DataTypes) / sizeof(s_Data.DataTypes[0]); i++)
		{
			stride += ShaderDataTypeSize(s_Data.DataTypes[i]);
		}
		uint32_t VAOoffset = 0;
		for (int i = 0; i < sizeof(s_Data.DataTypes) / sizeof(s_Data.DataTypes[0]); i++)
		{
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, GetComponentCount(s_Data.DataTypes[i]), GL_FLOAT, GL_FALSE, stride, (void*)(VAOoffset));
			VAOoffset += ShaderDataTypeSize(s_Data.DataTypes[i]);
		}


		// EBO
		uint32_t offset = 0;
		uint32_t* indices = new uint32_t[s_Data.MaxIndices];
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;
			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;
			offset += 4;
		}
		glCreateBuffers(1, &s_Data.EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, s_Data.MaxIndices * sizeof(uint32_t), indices, GL_STATIC_DRAW);
		delete[] indices;
		////////////////////////////////////////////////////////////////////

	}

	void Renderer::Shutdown()
	{

	}

	void Renderer::ClearScene()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::SetupRenderState()
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Renderer::Draw()
	{
		if (s_Data.IndexCount == 0)
			return;

		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.m_TextureBatch[i]->Bind(i);

		// Fill VBO
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.VertexDataPtr - (uint8_t*)s_Data.vertexDataArray);
		glBindBuffer(GL_ARRAY_BUFFER, s_Data.VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, s_Data.vertexDataArray);

		// Draw
		glBindVertexArray(s_Data.VAO);
		glDrawElements(GL_TRIANGLES, s_Data.IndexCount, GL_UNSIGNED_INT, nullptr);
		//glBindVertexArray(0);
	}

	void Renderer::NewBatch()
	{
		s_Data.IndexCount = 0;
		s_Data.VertexDataPtr = s_Data.vertexDataArray;
		s_Data.TextureSlotIndex = 1;
	}

	void Renderer::SetCamera(Camera camera)
	{
		s_Data.m_Shader.Bind();
		s_Data.m_Shader.setMat4("u_ViewProjection", camera.GetViewProjection());
		NewBatch();
	}

	void Renderer::AddQuad(const glm::mat4 transform, const glm::vec4 color, 
		const std::shared_ptr<Texture2D>& texture, const int entityID)
	{
		if (s_Data.IndexCount+6 > s_Data.MaxIndices)
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
			s_Data.VertexDataPtr->Position = transform * s_Data.vertexPositions[i];
			s_Data.VertexDataPtr->Color = color;
			s_Data.VertexDataPtr->TexCoord = s_Data.textureCoords[i];
			s_Data.VertexDataPtr->TexSlot = textureIndex;
			s_Data.VertexDataPtr->EntityID = entityID;
			s_Data.VertexDataPtr++;
		}

		s_Data.IndexCount += 6;
		s_Data.VertexCount += 4;
		s_Data.QuadCount += 1;
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