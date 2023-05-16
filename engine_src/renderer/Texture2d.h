#ifndef AU_TEXTURE2D_H
#define AU_TEXTURE2D_H

#include "au_renderer.h"
#include "ImageData.h"

namespace Audace {
	class Texture2d {
		GLuint glid;
		ImageData img;

		public:
		Texture2d(ImageData img) : img(img) {}
		void create();
		void bind(int texUnit);
		void destroy();
	};
}

#endif