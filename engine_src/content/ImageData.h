//
// Created by Suspense on 5/15/2023.
//

#ifndef AU_IMAGEDATA_H
#define AU_IMAGEDATA_H

#include "au_renderer.h"

namespace Audace
{
	class ImageData
	{
	public:
		unsigned char *bytes;
		int width;
		int height;
		GLenum format;
		GLenum internalFormat;
		GLenum type;

		ImageData() {}
		ImageData(unsigned char *bytes, int width, int height, GLenum format, GLenum internalFormat = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE)
			: bytes(bytes), width(width), height(height), format(format), internalFormat(internalFormat), type(type)
		{
		}

		void release();
	};
}

#endif // AU_IMAGEDATA_H
