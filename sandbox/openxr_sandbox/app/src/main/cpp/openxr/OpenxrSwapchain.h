//
// Created by Suspense on 3/2/2023.
//

#ifndef AUDACE_OPENXRSWAPCHAIN_H
#define AUDACE_OPENXRSWAPCHAIN_H

#include <vector>
#include <map>
#include <GLES/gl.h>
#include "glm/glm.hpp"

#include "openxr_common.h"

class OpenxrSwapchain {
	XrSwapchain handle;
	bool usesDepthBuffer;
	glm::ivec2 size;
	int64_t format;

	std::vector<XrSwapchainImageOpenGLESKHR> images;
	std::vector<GLuint> depthBuffers;

	GLuint makeDepthBuffer(GLuint colorBuffer);

public:
	bool init(XrSession session, uint32_t w, uint32_t h, int64_t f, bool useDepthBuffer = true);

	XrSwapchain getHandle() {return handle;}
	XrSwapchainImageOpenGLESKHR* getImage(uint32_t index) {return &images[index];}
	GLuint getDepthBuffer(GLuint index) {return depthBuffers[index];}

	bool startFrame();
	bool endFrame();

	glm::ivec2 getSize() { return size; }
};


#endif //AUDACE_OPENXRSWAPCHAIN_H
