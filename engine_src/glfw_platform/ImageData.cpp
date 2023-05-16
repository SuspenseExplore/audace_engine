#include "stb_image.h"
#include "ImageData.h"

namespace Audace
{
	void ImageData::release()
	{
		stbi_image_free(bytes);
		bytes = nullptr;
	}
}