#ifndef AU_FORWARDCAMERA_H
#define AU_FORWARDCAMERA_H

#include "au_renderer.h"
#include "math/Pose.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Audace {
	class ForwardCamera {
		Pose pose;
		glm::mat4 projMat;

		public:
		ForwardCamera(Pose pose, glm::mat4 projection) : pose(pose), projMat(projection) {
		}
		static ForwardCamera standard(glm::vec3 position, float viewWidth, float viewHeight) {
			glm::quat q = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
			glm::mat4 proj = glm::perspectiveFov(glm::radians(45.0f), viewWidth, viewHeight, 0.1f, 100.0f);
			return ForwardCamera(Pose(position, q), proj);
		}

		void move(float x, float y, float z) {
			pose.position += getRightVec() * x;
			pose.position += getFwdVec() * y;
			pose.position += getUpVec() * z;
		}

		void move(glm::vec3 v) {
			move(v.x, v.y, v.z);
		}

		glm::vec3 getPosition() {
			return pose.position;
		}

		void setPosition(float x, float y, float z) {
			pose.position.x = x;
			pose.position.y = y;
			pose.position.z = z;
		}

		void setPosition(glm::vec3 v) {
			pose.position = v;
		}

		void rotate(float x, float y, float z) {
			glm::quat q = glm::rotate(pose.orientation, x, getRightVec());
			q = glm::rotate(q, y, getFwdVec());
			q = glm::rotate(q, z, getUpVec());
			pose.orientation = q;
		}

		void setOrientation(float x, float y, float z) {
			pose.orientation = glm::quat(1, 0, 0, 0);
			rotate(x, y, z);
		}

		glm::vec3 getFwdVec() {
			glm::vec4 vec = glm::vec4(0, 1, 0, 0) * glm::mat4_cast(pose.orientation);
			return glm::vec3(vec);
		}

		glm::vec3 getRightVec() {
			glm::vec4 vec = glm::vec4(1, 0, 0, 0) * glm::mat4_cast(pose.orientation);
			return glm::vec3(vec);
		}

		glm::vec3 getUpVec() {
			glm::vec4 vec = glm::vec4(0, 0, 1, 0) * glm::mat4_cast(pose.orientation);
			return glm::vec3(vec);
		}

		glm::mat4 getViewMat() {
			glm::vec3 fwd = glm::vec4(0, 1, 0, 0) * glm::mat4_cast(pose.orientation);
			glm::vec3 up = glm::vec4(0, 0, 1, 0) * glm::mat4_cast(pose.orientation);
			glm::vec3 targetPos = pose.position + fwd;
			return glm::lookAt(pose.position, targetPos, up);
		}

		glm::mat4 getProjMat() {
			return projMat;
		}

		const glm::mat4 getvpMat() {
			return getProjMat() * getViewMat();
		}
	};
}

#endif