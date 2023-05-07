#ifndef AU_SHADERPROGRAM_H
#define AU_SHADERPROGRAM_H

#include <string>
#include "au_renderer.h"

namespace Audace {
	class ShaderProgram {
		GLuint glid;
		std::string vsSrc;
		std::string fsSrc;

		public:
		ShaderProgram(std::string vs, std::string fs) : vsSrc(vs), fsSrc(fs) {}
		void create();
		void destroy();
		void bind();

		static GLuint loadShader(const char *src, GLenum shaderType);
	};
}

#endif