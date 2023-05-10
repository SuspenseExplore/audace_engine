#include <iostream>
#include "ShaderProgram.h"
#include "AuLogger.h"

namespace Audace {
	void ShaderProgram::create() {
		GLuint vs = loadShader(vsSrc.c_str(), GL_VERTEX_SHADER);
		GLuint fs = loadShader(fsSrc.c_str(), GL_FRAGMENT_SHADER);
		glid = glCreateProgram();
		glAttachShader(glid, vs);
		glAttachShader(glid, fs);
		glLinkProgram(glid);
		GLint res;
		glGetProgramiv(glid, GL_LINK_STATUS, &res);
		if (!res) {
			glGetProgramiv(glid, GL_INFO_LOG_LENGTH, &res);
			char *log = new char[res];
			glGetProgramInfoLog(glid, res, nullptr, log);
			AU_ENGINE_LOG_ERROR("Shader program link error: {}", log);
			delete[] log;
			glDeleteProgram(glid);
		}

		glDeleteShader(vs);
		glDeleteShader(fs);
		AU_RENDERER_LOG_TRACE("Created ShaderProgram {}", glid);
	}

	void ShaderProgram::bind() {
		glUseProgram(glid);
		AU_RENDERER_LOG_TRACE("Bound ShaderProgram {}", glid);
	}

	void ShaderProgram::destroy() {
		glDeleteProgram(glid);
		AU_RENDERER_LOG_TRACE("Destroyed ShaderProgram {}", glid);
		glid = -1;
	}

	GLuint ShaderProgram::loadShader(const char *src, GLenum shaderType) {
		GLuint shaderId = glCreateShader(shaderType);
		glShaderSource(shaderId, 1, &src, nullptr);
		glCompileShader(shaderId);
		GLint res;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &res);
		if (!res) {
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &res);
			char *log = new char[res];
			glGetShaderInfoLog(shaderId, res, nullptr, log);
			AU_ENGINE_LOG_ERROR("Shader compile error: {}", log);
			delete[] log;
		}
		return shaderId;
	}
}