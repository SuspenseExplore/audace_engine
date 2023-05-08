//
// Created by Suspense on 3/2/2023.
//

#ifndef AUDACE_OPENXRSWAPCHAIN_H
#define AUDACE_OPENXRSWAPCHAIN_H

#include <vector>
#include <map>
#include <GLES/gl.h>

#include "openxr_common.h"

class OpenxrSwapchain {
	XrSwapchain handle;

	std::vector<XrSwapchainImageOpenGLESKHR> images;
	std::vector<GLuint> depthBuffers;

	GLuint makeDepthBuffer(GLuint colorBuffer);

public:
	bool init(XrSession session, uint32_t w, uint32_t h, int64_t f);

	XrSwapchain getHandle() {return handle;}
	XrSwapchainImageOpenGLESKHR* getImage(uint32_t index) {return &images[index];}
	GLuint getDepthBuffer(GLuint index) {return depthBuffers[index];}

	bool startFrame();
	bool endFrame();
};


#endif //AUDACE_OPENXRSWAPCHAIN_H
