#pragma once
#include <memory>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Greenbox {

	class Shader {

	public:
		Shader(const char* vertPath, const char* fragPath);
		~Shader() = default;

		void Bind();
		void Unbind();

		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setMat3(const std::string& name, glm::mat3 value) const;
		void setMat4(const std::string& name, glm::mat4 value) const;
		void setVec2(const std::string& name, glm::vec2 value) const;
		void setVec3(const std::string& name, glm::vec3 value) const;
		void setIntArray(const std::string& name, int* values, uint32_t count) const;

	private:
		uint32_t m_RendererID;

	};

}