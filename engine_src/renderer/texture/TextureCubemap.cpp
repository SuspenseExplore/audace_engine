#include "TextureCubemap.h"

namespace Audace
{
	void TextureCubemap::create()
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, imgs[0].format == GL_ALPHA ? 1 : 4);
		glGenTextures(1, &glid);
		AU_CHECK_GL_ERRORS();
		bind(0);

		for (int i = 0; i < 6; i++)
		{
			ImageData img = imgs[i];
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, img.internalFormat, img.width, img.height, 0, img.format, img.type, img.bytes);
			AU_CHECK_GL_ERRORS();
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		AU_CHECK_GL_ERRORS();
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		AU_CHECK_GL_ERRORS();
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		AU_CHECK_GL_ERRORS();
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		AU_CHECK_GL_ERRORS();
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		AU_CHECK_GL_ERRORS();
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		AU_CHECK_GL_ERRORS();
		// img.release();
	}

	void TextureCubemap::bind(int texUnit)
	{
		glActiveTexture(GL_TEXTURE0 + texUnit);
		AU_CHECK_GL_ERRORS();
		glBindTexture(GL_TEXTURE_CUBE_MAP, glid);
		AU_CHECK_GL_ERRORS();
	}

	void TextureCubemap::destroy()
	{
		glDeleteTextures(1, &glid);
		glid = -1;
	}
}