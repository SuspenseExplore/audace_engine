#include "GlUtil.h"

std::string getGlErrorString(int glErrCode)
{
	switch (glErrCode)
	{
	case 0x500:
		return "GL_INVALID_ENUM";

	case 0x501:
		return "GL_INVALID_VALUE";

	case 0x502:
		return "GL_INVALID_OPERATION";

	case 0x503:
		return "GL_STACK_OVERFLOW";

	case 0x504:
		return "GL_STACK_UNDERFLOW";

	case 0x505:
		return "GL_OUT_OF_MEMORY";

	case 0x506:
		return "GL_INVALID_FRAMEBUFFER_OPERATION";

	case 0x507:
		return "GL_CONTEXT_LOST";

	case 0x8031:
		return "GL_TABLE_TOO_LARGE";
	}
	return "GL_NO_ERROR";
}
