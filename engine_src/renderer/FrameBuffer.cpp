#include "FrameBuffer.h"
#include "texture/Texture2d.h"

namespace Audace
{
	void FrameBuffer::create()
	{
		glGenFramebuffers(1, &glid);
		AU_CHECK_GL_ERRORS();
		AU_RENDERER_LOG_TRACE("Created FrameBuffer {}", glid);
	}

	void FrameBuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, glid);
		AU_CHECK_GL_ERRORS();
		AU_RENDERER_LOG_TRACE("Bound FrameBuffer {}", glid);
	}

	void FrameBuffer::destroy()
	{
		glDeleteFramebuffers(1, &glid);
		AU_CHECK_GL_ERRORS();
		AU_RENDERER_LOG_TRACE("Destroyed FrameBuffer {}", glid);
		glid = -1;
	}

	void FrameBuffer::colorAttachment(Texture2d *tex)
	{
		bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->getId(), 0);
		texAttachment = tex;

		//TODO: this whole thing should be more flexible about what buffers exist, 
		// where they come from, who owns them, etc
		glGenRenderbuffers(1, &glidDepthStencil);
		glBindRenderbuffer(GL_RENDERBUFFER, glidDepthStencil);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH32F_STENCIL8, tex->getWidth(), tex->getHeight());
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, glidDepthStencil);
	}

	bool FrameBuffer::checkStatus()
	{
		GLenum r = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		switch (r)
		{
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			AU_RENDERER_LOG_ERROR("FrameBuffer {}: Incomplete attachment", glid);
			return false;

		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
			AU_RENDERER_LOG_ERROR("FrameBuffer {}: Incomplete dimensions", glid);
			return false;

		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
			AU_RENDERER_LOG_ERROR("FrameBuffer {}: Incomplete layer targets", glid);
			return false;

		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			AU_RENDERER_LOG_ERROR("FrameBuffer {}: Incomplete; missing attachment", glid);
			return false;

		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			AU_RENDERER_LOG_ERROR("FrameBuffer {}: Incomplete multisample", glid);
			return false;

		case GL_FRAMEBUFFER_COMPLETE:
			AU_RENDERER_LOG_TRACE("FrameBuffer {} is complete", glid);
			return true;
		}

		AU_RENDERER_LOG_ERROR("FrameBuffer {} STATUS UNKNOWN!!??!!??!?!??!", glid);
		return false;
	}
}