#include "gbpch.h"

#include "Shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Core/Log.h"

namespace Greenbox {

	Shader::Shader(const char* vertPath, const char* fragPath)
	{
		// read the files
		std::string vertCodeString;
		std::string fragCodeString;
		std::ifstream vertShaderFile;
		std::ifstream fragShaderFile;
		std::stringstream vertShaderStream;
		std::stringstream fragShaderStream;
		vertShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			vertShaderFile.open(vertPath);
			fragShaderFile.open(fragPath);
			vertShaderStream << vertShaderFile.rdbuf();
			fragShaderStream << fragShaderFile.rdbuf();
			vertShaderFile.close();
			fragShaderFile.close();
			vertCodeString = vertShaderStream.str();
			fragCodeString = fragShaderStream.str();
		} catch (const std::exception&) {
			GB_ASSERT(false, "Failed to read from the file");
		}
		const char* vertCode = vertCodeString.c_str();;
		const char* fragCode = fragCodeString.c_str();


		// create shaders
		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertCode, NULL);
		glCompileShader(vertexShader);
		int vertexCompilationSuccess;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexCompilationSuccess);
		if (vertexCompilationSuccess == GL_FALSE) {
			GLint infoLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLength);
			std::vector<GLchar> vertexInfoLog(infoLength);
			glGetShaderInfoLog(vertexShader, infoLength, &infoLength, &vertexInfoLog[0]);
			glDeleteShader(vertexShader);
			GB_ERROR("{0}", vertexInfoLog.data());
			GB_ASSERT(false, "Vertex Shader Compilation Failed");
		}

		unsigned int fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragCode, NULL);
		glCompileShader(fragmentShader);
		int fragmentCompilationSuccess;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentCompilationSuccess);
		if (fragmentCompilationSuccess == GL_FALSE) {
			GLint infoLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLength);
			std::vector<GLchar> fragmentInfoLog(infoLength);
			glGetShaderInfoLog(fragmentShader, infoLength, &infoLength, &fragmentInfoLog[0]);
			glDeleteShader(fragmentShader);
			GB_ERROR("{0}", fragmentInfoLog.data());
			GB_ASSERT(false, "Fragment Shader Compilation Failed");
		}


		// create program
		m_RendererID = glCreateProgram();
		glAttachShader(m_RendererID, vertexShader);
		glAttachShader(m_RendererID, fragmentShader);
		glLinkProgram(m_RendererID);
		int programLinkSuccess;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*)&programLinkSuccess);
		if (programLinkSuccess == GL_FALSE)
		{
			GLint infoLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &infoLength);
			std::vector<GLchar> programInfoLog(infoLength);
			glGetProgramInfoLog(m_RendererID, infoLength, &infoLength, &programInfoLog[0]);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			glDeleteProgram(m_RendererID);
			GB_ERROR("{0}", programInfoLog.data());
			GB_ASSERT(false, "Shader Link Failed");
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void Shader::Bind()
	{
		glUseProgram(m_RendererID);
	}

	void Shader::Unbind()
	{
		glUseProgram(0);
	}

	void Shader::setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), (int)value);
	}

	void Shader::setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), value);
	}

	void Shader::setFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(m_RendererID, name.c_str()), value);
	}

	void Shader::setMat3(const std::string& name, glm::mat3 value) const {
		glUniformMatrix3fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::setMat4(const std::string& name, glm::mat4 value) const {
		glUniformMatrix4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::setVec2(const std::string& name, glm::vec2 value) const {
		glUniform2fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, glm::value_ptr(value));
	}

	void Shader::setVec3(const std::string& name, glm::vec3 value) const {
		glUniform3fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, glm::value_ptr(value));
	}

	void Shader::setIntArray(const std::string& name, int* values, uint32_t count) const {
		glUniform1iv(glGetUniformLocation(m_RendererID, name.c_str()), count, values);
	}
}
