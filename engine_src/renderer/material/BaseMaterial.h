#ifndef AU_BASEMATERIAL_H
#define AU_BASEMATERIAL_H

#include "renderer/ShaderProgram.h"

namespace Audace {
	class BaseMaterial {
		public:
		virtual ShaderProgram* getShader() = 0;
		virtual void setShader(ShaderProgram* shader) = 0;
		virtual void apply() = 0;
	};
}

#endif