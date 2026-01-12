#ifndef AU_FRAMEBUFFER_H
#define AU_FRAMEBUFFER_H

#include "au_renderer.h"

namespace Audace
{
	class Texture2d;

	class FrameBuffer
	{
		GLuint glid;
		GLuint glidDepthStencil;
		Texture2d *texAttachment = nullptr;

		public:
		FrameBuffer() {}
		
		void create();
		void bind();
		void destroy();

		void colorAttachment(Texture2d *tex);
		Texture2d* getColorTexAttachment() { return texAttachment; }

		bool checkStatus();
	};
}

#endif