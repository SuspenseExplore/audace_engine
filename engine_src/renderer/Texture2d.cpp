#include "Texture2d.h"

namespace Audace {
	void Texture2d::create() {
		glPixelStorei(GL_UNPACK_ALIGNMENT, img.format == GL_ALPHA ? 1 : 4);
		glGenTextures(1, &glid);
		AU_CHECK_GL_ERRORS();
		bind(0);
		glTexImage2D(GL_TEXTURE_2D, 0, img.format, img.width, img.height, 0, img.format, GL_UNSIGNED_BYTE, img.bytes);
		AU_CHECK_GL_ERRORS();
		glGenerateMipmap(GL_TEXTURE_2D);
		AU_CHECK_GL_ERRORS();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		AU_CHECK_GL_ERRORS();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		AU_CHECK_GL_ERRORS();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		AU_CHECK_GL_ERRORS();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		AU_CHECK_GL_ERRORS();

		// img.release();
	}

	void Texture2d::bind(int texUnit) {
		glActiveTexture(GL_TEXTURE0 + texUnit);
		AU_CHECK_GL_ERRORS();
		glBindTexture(GL_TEXTURE_2D, glid);
		AU_CHECK_GL_ERRORS();
	}
}