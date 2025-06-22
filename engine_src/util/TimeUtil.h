#ifndef AU_TIMEUTIL_H
#define AU_TIMEUTIL_H

#include <chrono>

namespace Audace
{
	class TimeUtil
	{
	public:
		static long long currentTimeMillis()
		{
			return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

		}
	};
}

#endif