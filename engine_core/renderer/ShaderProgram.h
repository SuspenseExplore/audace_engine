#ifndef AU_SHADERPROGRAM_H
#define AU_SHADERPROGRAM_H

#include <string>
#include "au_platform.h"

namespace Audace {
	class ShaderProgram {
		GLuint glid = -1;
		std::string vsSrc;
		std::string fsSrc;

		public:
		ShaderProgram(std::string vs, std::string fs) : vsSrc(vs), fsSrc(fs) {}

		void create();
		void bind();
		void unbind();
		void destroy();

		GLuint getId() {return glid;}

		static GLuint loadShader(const char *src, GLenum type);
	};
}

#endif