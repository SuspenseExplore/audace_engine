#ifndef AU_ITEXTURE_H
#define AU_ITEXTURE_H

namespace Audace
{
	class ITexture
	{
	public:
		virtual void create() = 0;
		virtual void bind(int texUnit) = 0;
		virtual void destroy() = 0;
	};
}

#endif