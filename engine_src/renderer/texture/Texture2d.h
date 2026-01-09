#ifndef AU_TEXTURE2D_H
#define AU_TEXTURE2D_H

#include "au_renderer.h"
#include "ITexture.h"
#include "content/ImageData.h"

namespace Audace
{
	class Texture2d : public ITexture
	{
		GLuint glid;
		ImageData img;

	public:
		Texture2d(ImageData img) : img(img) {}
		void create() override;
		void bind(int texUnit) override;
		void destroy() override;

		GLuint getId() { return glid; }
		int getWidth() { return img.width; }
		int getHeight() { return img.height; }
	};
}

#endif