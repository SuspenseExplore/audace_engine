//
// Created by Suspense on 5/18/2025.
//

#include "AimIndicator.h"
#include "renderer/Mesh.h"
#include "renderer/Shapes.h"
#include "renderer/ShaderProgram.h"
#include "renderer/material/BaseMaterial.h"
#include "scene/BaseCamera.h"
#include "scene/Scene.h"

AimIndicator::AimIndicator()
		: Audace::Sprite({Audace::Shapes::cubePosNormTan()}) {
}

void AimIndicator::handlePoseEvent(Audace::PoseInputEvent e) {
	if (e.changed) {
		Audace::Pose p = e.state;
		pose = p;
	}
}

void AimIndicator::renderWorldSpace(Audace::Scene *scene) {
	getMesh()->getMaterial()->getShader()->bind();
	getMesh()->getMaterial()->getShader()->setUniformVec4("offset", {offset, 0.0});
	setPosition(pose.position + scene->getCamera()->getOriginPos());
	setOrientation(pose.orientation);
	setScale(scale);
	Audace::Sprite::renderWorldSpace(scene);
	getMesh()->getMaterial()->getShader()->setUniformVec4("offset", {0.0, 0.0, 0.0, 0.0});
}
