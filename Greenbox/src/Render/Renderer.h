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

		static void PrintTextureInBatch();

	private:
		static int HasTextureID(uint32_t RendererID);
	};
}