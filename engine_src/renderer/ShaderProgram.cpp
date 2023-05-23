#include <iostream>
#include "ShaderProgram.h"
#include "AuLogger.h"

namespace Audace
{
	void ShaderProgram::create()
	{
		GLuint vs = loadShader(vsSrc.c_str(), GL_VERTEX_SHADER);
		GLuint fs = loadShader(fsSrc.c_str(), GL_FRAGMENT_SHADER);
		glid = glCreateProgram();
		glAttachShader(glid, vs);
		AU_CHECK_GL_ERRORS();

		glAttachShader(glid, fs);
		AU_CHECK_GL_ERRORS();
		glLinkProgram(glid);
		AU_CHECK_GL_ERRORS();
		GLint res;
		glGetProgramiv(glid, GL_LINK_STATUS, &res);
		AU_CHECK_GL_ERRORS();
		if (!res)
		{
			glGetProgramiv(glid, GL_INFO_LOG_LENGTH, &res);
			AU_CHECK_GL_ERRORS();
			char *log = new char[res];
			glGetProgramInfoLog(glid, res, nullptr, log);
			AU_CHECK_GL_ERRORS();
			AU_ENGINE_LOG_ERROR("Shader program link error: {}", log);
			delete[] log;
			glDeleteProgram(glid);
			AU_CHECK_GL_ERRORS();
		}

		glDeleteShader(vs);
		AU_CHECK_GL_ERRORS();
		glDeleteShader(fs);
		AU_CHECK_GL_ERRORS();

		findUniforms();
		AU_RENDERER_LOG_TRACE("Created ShaderProgram {}", glid);
	}

	void ShaderProgram::bind()
	{
		glUseProgram(glid);
		AU_CHECK_GL_ERRORS();
		AU_RENDERER_LOG_TRACE("Bound ShaderProgram {}", glid);
	}

	void ShaderProgram::destroy()
	{
		glDeleteProgram(glid);
		AU_CHECK_GL_ERRORS();
		AU_RENDERER_LOG_TRACE("Destroyed ShaderProgram {}", glid);
		glid = -1;
	}

	void ShaderProgram::findUniforms()
	{
		GLint count;
		glGetProgramiv(glid, GL_ACTIVE_UNIFORMS, &count);
		AU_CHECK_GL_ERRORS();
		AU_RENDERER_LOG_TRACE("Loading {} uniforms in shader {}", count, glid);

		GLint nameLength;
		glGetProgramiv(glid, GL_ACTIVE_UNIFORM_MAX_LENGTH, &nameLength);
		AU_CHECK_GL_ERRORS();
		char *name = new char[nameLength];
		AU_RENDERER_LOG_TRACE("Uniform name max length: {}", nameLength);

		for (int i = 0; i < count; i++)
		{
			GLsizei length;
			GLsizei size;
			GLenum type;
			glGetActiveUniform(glid, i, nameLength, &length, &size, &type, name);
			AU_CHECK_GL_ERRORS();
			GLint location = glGetUniformLocation(glid, name);
			AU_CHECK_GL_ERRORS();
			AU_RENDERER_LOG_TRACE("Uniform {} - name: {}, size: {}, type: {}, location: {}", i, name, size, type, location);
			uniforms[std::string(name, length)] = location;
		}

		delete[] name;
	}

	void ShaderProgram::setUniformInt(std::string name, int value)
	{
		glUniform1i(uniforms[name], value);
		AU_CHECK_GL_ERRORS();
		AU_RENDERER_LOG_TRACE("Set int uniform in shader {} at location {} to value {}", glid, uniforms[name], value);
	}

	void ShaderProgram::setUniformVec2(std::string name, float x, float y) {
		glUniform2f(uniforms[name], x, y);
		AU_CHECK_GL_ERRORS();
		AU_RENDERER_LOG_TRACE("Set vec2 uniform in shader {} at location {} to value [{},{}]", glid, uniforms[name], x, y);
	}

	void ShaderProgram::setUniformVec2(std::string name, float *value) {
		glUniform2f(uniforms[name], value[0], value[1]);
		AU_CHECK_GL_ERRORS();
		AU_RENDERER_LOG_TRACE("Set vec2 uniform in shader {} at location {} to value [{},{}]", glid, uniforms[name], value[0], value[1]);
	}

	void ShaderProgram::setUniformMat4(std::string name, float *value) {
		glUniformMatrix4fv(uniforms[name], 1, false, value);
		AU_CHECK_GL_ERRORS();
	}

	GLuint ShaderProgram::loadShader(const char *src, GLenum shaderType)
	{
		GLuint shaderId = glCreateShader(shaderType);
		glShaderSource(shaderId, 1, &src, nullptr);
		AU_CHECK_GL_ERRORS();
		glCompileShader(shaderId);
		AU_CHECK_GL_ERRORS();
		GLint res;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &res);
		AU_CHECK_GL_ERRORS();
		if (!res)
		{
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &res);
			AU_CHECK_GL_ERRORS();
			char *log = new char[res];
			glGetShaderInfoLog(shaderId, res, nullptr, log);
			AU_CHECK_GL_ERRORS();
			AU_ENGINE_LOG_ERROR("Shader compile error: {}", log);
			delete[] log;
		}
		return shaderId;
	}
}