//
// Created by Suspense on 5/15/2023.
//

#include "ImageData.h"

namespace Audace {
	void ImageData::release() {
		free(bytes);
	}
}