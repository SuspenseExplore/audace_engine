#include "RectGrid.h"

namespace Audace
{
	void RectGrid::setOrigin(float x, float y, float z) {
		origin.x = x;
		origin.y = y;
		origin.z = z;
	}

	void RectGrid::setCellSize(float x, float y, float z) {
		cellSize.x = x;
		cellSize.y = y;
		cellSize.z = z;
	}
	void RectGrid::setPadding(float x, float y, float z) {
		padding.x = x;
		padding.y = y;
		padding.z = z;
	}

	float RectGrid::getCellPosition(int x) {
		return getCellPosition(x, 0, 0).x;
	}
	
	glm::vec2 RectGrid::getCellPosition(int x, int y) {
		glm::vec3 cell = getCellPosition(x, y, 0);
		return glm::vec2(cell);
	}

	glm::vec3 RectGrid::getCellPosition(int x, int y, int z) {
		glm::vec3 cell = origin;
		cell.x += (x * cellSize.x);
		cell.x += ((x - 1) * padding.x);
		cell.y += (y * cellSize.y);
		cell.y += ((y - 1) * padding.y);
		cell.z += (z * cellSize.z);
		cell.z += ((z - 1) * padding.z);

		return cell;
	}

}