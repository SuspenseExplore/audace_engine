#ifndef AU_TEXTURECUBEMAP_H
#define AU_TEXTURECUBEMAP_H

#include "au_renderer.h"
#include "ITexture.h"
#include "content/ImageData.h"
#include <vector>

namespace Audace
{
	class TextureCubemap : public ITexture
	{
		GLuint glid;
		std::vector<ImageData> imgs;

	public:
		TextureCubemap(std::vector<ImageData> imgs) : imgs(imgs) {}
		void create() override;
		void bind(int texUnit) override;
		void destroy() override;
	};
}

#endif