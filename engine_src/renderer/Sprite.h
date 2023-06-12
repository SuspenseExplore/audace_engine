//
// Created by Spencer on 5/22/2023.
//

#ifndef AU_SPRITE_H
#define AU_SPRITE_H

#include <vector>
#include "au_renderer.h"
#include "content/Model.h"
#include "renderer/Mesh.h"
#include "renderer/VertexArray.h"

namespace Audace {

	/**
	 * Combines one or more Meshes with world transformation data to make a single coherent renderable object
	 */
	class Sprite {
		VertexArray* vertexArray;
		std::vector<Mesh*> meshes;

	public:
		Sprite(Model* model);

		void render();
	};

} // Audace

#endif //AU_SPRITE_H
