#ifndef AU_SHADERPROGRAM_H
#define AU_SHADERPROGRAM_H

#include <string>
#include <map>
#include "au_renderer.h"
#include "glm/glm.hpp"

namespace Audace {
	class ShaderProgram {
		GLuint glid = -1;
		std::string vsSrc;
		std::string fsSrc;
		std::map<std::string, GLint> uniforms;

		void findUniforms();

		public:
		ShaderProgram(std::string vs, std::string fs) : vsSrc(vs), fsSrc(fs) {}
		void create();
		void destroy();
		void bind();

		GLuint getId() {return glid;}

		void setUniformInt(std::string name, int value);
		void setUniformFloat(std::string name, float x);
		void setUniformVec2(std::string name, float x, float y);
		void setUniformVec2(std::string name, float *value);
		void setUniformVec3(std::string name, float x, float y, float z);
		void setUniformVec3(std::string name, glm::vec3 value);
		void setUniformVec3(std::string name, float *value);
		void setUniformVec4(std::string name, float x, float y, float z, float w);
		void setUniformVec4(std::string name, float *value);
		void setUniformMat4(std::string name, glm::mat4 mat);
		void setUniformMat4(std::string name, float* value);

		static GLuint loadShader(const char *src, GLenum shaderType);
	};
}

#endif