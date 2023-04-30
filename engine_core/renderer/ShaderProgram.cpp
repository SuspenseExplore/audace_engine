#include "RenderContext.h"
#include "ShaderProgram.h"

namespace Audace {
	void ShaderProgram::create() {
		GLuint vs = loadShader(vsSrc.c_str(), GL_VERTEX_SHADER);
		GLuint fs = loadShader(fsSrc.c_str(), GL_FRAGMENT_SHADER);
		glid = glCreateProgram();
		CALL_GL(glAttachShader, glid, vs);
		CALL_GL(glAttachShader, glid, fs);
		CALL_GL(glLinkProgram, glid);
		GLint res = 0;
		CALL_GL(glGetProgramiv, glid, GL_LINK_STATUS, &res);
		if (!res) {
			CALL_GL(glGetProgramiv, glid, GL_INFO_LOG_LENGTH, &res);
			char *log = new char[res];
			CALL_GL(glGetProgramInfoLog, glid, res, nullptr, log);
			AU_ENGINE_LOG_ERROR("Error linking shader program: {}", log);
			delete[] log;
			CALL_GL(glDeleteProgram, glid);
		}
		CALL_GL(glDeleteShader, vs);
		CALL_GL(glDeleteShader, fs);
	}

	void ShaderProgram::bind() {
		CALL_GL(glUseProgram, glid);
	}

	void ShaderProgram::unbind() {
		CALL_GL(glUseProgram, 0);
	}

	void ShaderProgram::destroy() {
		CALL_GL(glDeleteProgram, glid);
	}

	GLuint ShaderProgram::loadShader(const char *src, GLenum type) {
		GLuint id = glCreateShader(type);
		CALL_GL(glShaderSource, id, 1, &src, nullptr);
		CALL_GL(glCompileShader, id);
		GLint res = 0;
		CALL_GL(glGetShaderiv, id, GL_COMPILE_STATUS, &res);
		if (!res) {
			CALL_GL(glGetShaderiv, id, GL_INFO_LOG_LENGTH, &res);
			char *log = new char[res];
			CALL_GL(glGetShaderInfoLog, id, res, nullptr, log);
			AU_ENGINE_LOG_ERROR("Error compiling shader: {}", log);
			delete[] log;
			CALL_GL(glDeleteShader, id);

			return 0;
		}
		return id;
	}
}