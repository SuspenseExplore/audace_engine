//
// Created by Suspense on 3/3/2023.
//

#include "OpenxrView.h"

bool OpenxrView::init(XrSession session, XrViewConfigurationView cfg, int64_t f) {
	config = cfg;
	width = static_cast<int32_t>(cfg.recommendedImageRectWidth);
	height = static_cast<int32_t>(cfg.recommendedImageRectHeight);
	return swapchain.init(session, width, height, f);
}