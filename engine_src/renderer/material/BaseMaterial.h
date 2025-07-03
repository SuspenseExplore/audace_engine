#ifndef AU_BASEMATERIAL_H
#define AU_BASEMATERIAL_H

#include <string>

namespace Audace {
	class ShaderProgram;
	
	class BaseMaterial {
		public:
		virtual std::string getName() = 0;
		virtual ShaderProgram* getShader() = 0;
		virtual void setShader(ShaderProgram* shader) = 0;
		virtual void apply() = 0;
	};
}

#endif