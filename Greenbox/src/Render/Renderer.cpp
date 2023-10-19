#include "gbpch.h"
#include "Renderer.h"

namespace Greenbox {

	Renderer::Renderer()
		: m_Shader("assets/shaders/shader.vert", "assets/shaders/shader.frag")
	{
		vertexDataArray = new vertexData[MaxVertices];

		// Texture
		//White Texture
		m_TextureBatchPaths[0] = "";
		m_TextureBatch[0] = std::make_shared<Texture2D>(0xffffffff, 1, 1);
		// initialize slots
		m_Shader.Bind();
		int32_t samplers[MaxTextureSlots];
		for (uint32_t i = 0; i < MaxTextureSlots; i++)
			samplers[i] = i;
		m_Shader.setIntArray("u_Texture", samplers, MaxTextureSlots);



		///////////////////////////////////////////////////////
		// VBO
		glCreateBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// Initially empty. Filled in Draw().
		glBufferData(GL_ARRAY_BUFFER, MaxVertices * sizeof(vertexData), nullptr, GL_DYNAMIC_DRAW);




		// VAO
		glCreateVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		uint32_t stride = 0;
		for (int i = 0; i < sizeof(DataTypes) / sizeof(DataTypes[0]); i++)
		{
			stride += ShaderDataTypeSize(DataTypes[i]);
		}
		uint32_t VAOoffset = 0;
		for (int i = 0; i < sizeof(DataTypes) / sizeof(DataTypes[0]); i++)
		{
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, GetComponentCount(DataTypes[i]), GL_FLOAT, GL_FALSE, stride, (void*)(VAOoffset));
			VAOoffset += ShaderDataTypeSize(DataTypes[i]);
		}

		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(0));
		//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
		//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(7 * sizeof(float)));
		//glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(9 * sizeof(float)));
		//glEnableVertexAttribArray(0);
		//glEnableVertexAttribArray(1);
		//glEnableVertexAttribArray(2);
		//glEnableVertexAttribArray(3);




		// EBO
		uint32_t offset = 0;
		uint32_t* indices = new uint32_t[MaxIndices];
		for (uint32_t i = 0; i < MaxIndices; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;
			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;
			offset += 4;
		}
		glCreateBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, MaxIndices * sizeof(uint32_t), indices, GL_STATIC_DRAW);
		delete[] indices;
		////////////////////////////////////////////////////////////////////

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
		if (IndexCount == 0)
			return;

		for (uint32_t i = 0; i < TextureSlotIndex; i++)
			m_TextureBatch[i]->Bind(i);

		// Fill VBO
		uint32_t dataSize = (uint32_t)((uint8_t*)VertexDataPtr - (uint8_t*)vertexDataArray);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, vertexDataArray);

		// Draw
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, nullptr);
		//glBindVertexArray(0);
	}

	void Renderer::NewBatch()
	{
		IndexCount = 0;
		VertexDataPtr = vertexDataArray;
		TextureSlotIndex = 1;
	}

	void Renderer::SetCamera(Camera camera)
	{
		m_Shader.Bind();
		m_Shader.setMat4("u_ViewProjection", camera.GetViewProjection());
		NewBatch();
	}

	void Renderer::AddQuad(glm::mat4 transform, const glm::vec4 color, const std::shared_ptr<Texture2D>& texture)
	{
		if (IndexCount+6 > MaxIndices)
		{
			Draw();
			NewBatch();
		}

		uint32_t textureIndex;
		int stored = HasTextureID(texture.get()->GetRendererID());
		if (stored == -1)
		{
			m_TextureBatchPaths[TextureSlotIndex] = texture->GetPath();
			m_TextureBatch[TextureSlotIndex] = texture;
			textureIndex = TextureSlotIndex;
			TextureSlotIndex += 1;
		}
		else {
			textureIndex = stored;
		}

		for (size_t i = 0; i < 4; i++)
		{
			VertexDataPtr->Position = transform * vertexPositions[i];
			VertexDataPtr->Color = color;
			VertexDataPtr->TexCoord = textureCoords[i];
			VertexDataPtr->TexSlot = textureIndex;
			VertexDataPtr++;
		}

		IndexCount += 6;
		VertexCount += 4;
		QuadCount += 1;
	}


	void Renderer::AddQuad(glm::mat4 transform, glm::vec4 color, std::string texturePath)
	{
		if (IndexCount + 6 > MaxIndices)
		{
			Draw();
			NewBatch();
		}

		

		uint32_t textureIndex;

		if (texturePath == "")
		{
			textureIndex = 0;
		}
		else {
			int storedInRepo = HasTextureInRepo(texturePath);
			if (storedInRepo == -1)	// if not stored in repo
			{
				// store in repo
				m_TextureRepoPaths[TextureRepoIndex] = texturePath;
				m_TextureRepo[TextureRepoIndex] = std::make_shared<Texture2D>(texturePath.c_str());
				//GB_INFO("Renderer::AddQuad    A new Texture is created. There are {0} textures now", TextureRepoIndex+1);

				// add into batch
				m_TextureBatchPaths[TextureSlotIndex] = texturePath;
				m_TextureBatch[TextureSlotIndex] = m_TextureRepo[TextureRepoIndex];
				textureIndex = TextureSlotIndex;

				TextureRepoIndex += 1;
				TextureSlotIndex += 1;
			}
			else {	// already stored in repo
				int storedInBatch = HasTextureInBatch(texturePath);
				if (storedInBatch == -1)	// not added into batch
				{
					// find the texture pointer in repo
					m_TextureBatch[TextureSlotIndex] = m_TextureRepo[storedInRepo];
					textureIndex = TextureSlotIndex;
					TextureSlotIndex += 1;
				}
				else {				// already in batch
					textureIndex = storedInBatch;
				}
			}
		}

		for (size_t i = 0; i < 4; i++)
		{
			VertexDataPtr->Position = transform * vertexPositions[i];
			VertexDataPtr->Color = color;
			VertexDataPtr->TexCoord = textureCoords[i];
			VertexDataPtr->TexSlot = textureIndex;
			VertexDataPtr++;
		}

		IndexCount += 6;
		VertexCount += 4;
		QuadCount += 1;
	}

}