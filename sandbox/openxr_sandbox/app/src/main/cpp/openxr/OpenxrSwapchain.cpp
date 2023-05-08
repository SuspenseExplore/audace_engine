//
// Created by Suspense on 3/2/2023.
//

#include <GLES2/gl2.h>
#include <GLES3/gl31.h>

#include "openxr_common.h"
#include "OpenxrSwapchain.h"

bool OpenxrSwapchain::init(XrSession session, uint32_t width, uint32_t height, int64_t format) {
	XrSwapchainCreateInfo swapchainCreateInfo{XR_TYPE_SWAPCHAIN_CREATE_INFO};
	swapchainCreateInfo.arraySize = 1;
	swapchainCreateInfo.format = format;
	swapchainCreateInfo.width = width;
	swapchainCreateInfo.height = height;
	swapchainCreateInfo.mipCount = 1;
	swapchainCreateInfo.faceCount = 1;
	swapchainCreateInfo.sampleCount = 1;
	swapchainCreateInfo.usageFlags =
			XR_SWAPCHAIN_USAGE_SAMPLED_BIT | XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT;

	XR_ERROR_BAIL("xrCreateSwapchain", xrCreateSwapchain(session, &swapchainCreateInfo, &handle));

	uint32_t imageCount;
	XR_ERROR_BAIL("xrEnumerateSwapchainImages",
				  xrEnumerateSwapchainImages(handle, 0, &imageCount, nullptr));

	// XXX This should really just return XrSwapchainImageBaseHeader*
	images.resize(imageCount, {XR_TYPE_SWAPCHAIN_IMAGE_OPENGL_ES_KHR});
	XR_ERROR_BAIL("xrEnumerateSwapchainImages",
				  xrEnumerateSwapchainImages(handle, imageCount, &imageCount,
											 reinterpret_cast<XrSwapchainImageBaseHeader *>(&images[0])));

	for (XrSwapchainImageOpenGLESKHR img: images) {
		depthBuffers.push_back(makeDepthBuffer(img.image));
	}

	return true;
}

GLuint OpenxrSwapchain::makeDepthBuffer(GLuint colorBuffer) {
	GLint w;
	GLint h;
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);

	uint32_t depthTexture;
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, w, h, 0, GL_DEPTH_COMPONENT,
				 GL_UNSIGNED_INT, nullptr);

	return depthTexture;
}

bool OpenxrSwapchain::startFrame() {
	XrSwapchainImageAcquireInfo acquireInfo{XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO};

	uint32_t swapchainImageIndex;
	XR_ERROR_BAIL("xrAcquireSwapchainImage",
				  xrAcquireSwapchainImage(handle, &acquireInfo, &swapchainImageIndex));

	XrSwapchainImageWaitInfo waitInfo{XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO};
	waitInfo.timeout = XR_INFINITE_DURATION;
	XR_ERROR_BAIL("xrWaitSwapchainImage", xrWaitSwapchainImage(handle, &waitInfo));

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
						   getImage(swapchainImageIndex)->image,
						   0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
						   getDepthBuffer(swapchainImageIndex), 0);

	return true;
}

bool OpenxrSwapchain::endFrame() {
	XrSwapchainImageReleaseInfo releaseInfo{XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO};
	XR_ERROR_BAIL("xrReleaseSwapchainImage", xrReleaseSwapchainImage(handle, &releaseInfo));

	return true;
}