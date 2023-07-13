#ifndef AU_RECTGRID_H
#define AU_RECTGRID_H

#include "glm/glm.hpp"

namespace Audace {
	class RectGrid {
		glm::vec3 origin = glm::vec3(0, 0, 0);
		glm::vec3 cellSize = glm::vec3(1, 1, 1);
		glm::vec3 padding = glm::vec3(0, 0, 0);

		public:
		void setOrigin(float x, float y = 0.0f, float z = 0.0f);
		void setCellSize(float x, float y = 1.0f, float z = 1.0f);
		void setPadding(float x, float y = 0.0f, float z = 0.0f);

		float getCellPosition(int x);
		glm::vec2 getCellPosition(int x, int y);
		glm::vec3 getCellPosition(int x, int y, int z);
	};
}

#endif