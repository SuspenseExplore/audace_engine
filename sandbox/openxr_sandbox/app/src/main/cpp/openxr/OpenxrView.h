//
// Created by Suspense on 3/3/2023.
//

#ifndef AUDACE_OPENXRVIEW_H
#define AUDACE_OPENXRVIEW_H

#include "openxr_common.h"
#include "openxr/OpenxrSwapchain.h"

class OpenxrView {
	XrView viewData{XR_TYPE_VIEW};
	XrViewConfigurationView config;
	OpenxrSwapchain swapchain;

	int32_t width;
	int32_t height;

public:
	bool init(XrSession session, XrViewConfigurationView cfg, int64_t f);

	XrView getViewData() {return viewData;}
	void setViewData(XrView data) {viewData = data;}
	int32_t getWidth() {return width;}
	int32_t getHeight() {return height;}
	OpenxrSwapchain getSwapchain() {return swapchain;}
};


#endif //AUDACE_OPENXRVIEW_H
