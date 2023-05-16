//
// Created by Suspense on 5/15/2023.
//

#ifndef AU_IMAGEDATA_H
#define AU_IMAGEDATA_H

#include "au_renderer.h"

namespace Audace {
	class ImageData {
	public:
		unsigned char *bytes;
		int width;
		int height;
		GLenum format;

		ImageData(unsigned char *bytes, int width, int height, GLenum format)
				: bytes(bytes), width(width), height(height), format(format) {}

		void release();
	};
}

#endif //AU_IMAGEDATA_H
