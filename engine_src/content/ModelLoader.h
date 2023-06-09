#ifndef AU_MODELLOADER_H
#define AU_MODELLOADER_H

#include <string>
#include "content/Model.h"

namespace Audace {
	class ModelLoader {
		public:
		static Model *loadObj(std::string& fileContent);
	};
}

#endif