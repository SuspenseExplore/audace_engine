#ifndef AU_SHADERPROGRAM_H
#define AU_SHADERPROGRAM_H

#include <string>
#include <map>
#include "au_renderer.h"

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
		void setUniformMat4(std::string name, float *value);

		static GLuint loadShader(const char *src, GLenum shaderType);
	};
}

#endif