//
// Created by Suspense on 5/15/2023.
//

#include "stb_image.h"
#include "ImageData.h"

namespace Audace {
	void ImageData::release() {
		stbi_image_free(bytes);
	}
}