#include "GltfViewerScene.h"
#include "content/AssetStore.h"
#include "content/gltf/GltfLoader.h"
#include "renderer/material/PbrMetalRoughMat.h"
#include "scene/graph/RotationAnimation.h"
#include <vector>
using std::vector;

void GltfViewerScene::loadAssets(Audace::IFileAccess* fileLoader)
{
	Audace::BaseMaterial* mat = Audace::AssetStore::simpleBillboardMaterial();

	Audace::GltfLoader loader;
	// std::string path = "models/quaternius/medieval_village/";
	// std::string filename = "Overhang_RoofIncline_UnevenBricks.gltf";
	// loader.setImageLoadPath("images/quaternius/");
	std::string path = "models/_test/";
	std::string filename = "WaterBottle.gltf";
	loader.setImageLoadPath("images/_test/");
	loader.loadFile(fileLoader, path, filename);
	sceneGraph = loader.getSceneGraph(this);

	Audace::RotationAnimation* anim = new Audace::RotationAnimation();
	vector<float> times = { 0.0, 0.25, 0.5, 0.75, 1.0 };
	vector<glm::quat> values = {
		{0.0,   0.0,	0.0,     1.0},
		{0.0, 0.707,	0.0,   0.707},
		{0.0,   1.0,	0.0,     0.0},
		{0.0, 0.707,	0.0,  -0.707},
		{0.0,   0.0,	0.0,     1.0}
	};
	anim->setFrameTimes(times);
	anim->setFrameStates(values);
	anim->setTimeFactor(0.1);
	Audace::SceneGraphNode* node = sceneGraph->getRootNode();
	node->addAnimation(anim);

	ptLight = new Audace::PointLight();
	ptLight->setColor({ 1, 1, 1 });
	ptLight->setIntensity(10);
	ptLight->setPosition({ 2, 0.5, 1 });
}

void GltfViewerScene::render()
{
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera->update();
	sceneGraph->update(this);
	for (Audace::Sprite* s : sprites)
	{
		Audace::ShaderProgram* shader = s->getMesh()->getMaterial()->getShader();
		shader->bind();
		shader->setUniformVec3("viewPos", camera->getPosition());
		shader->setUniformVec4("ambientLight", 1, 1, 1, 0.2);
		shader->setUniformVec3("ptLight[0].position", ptLight->getPosition());
		shader->setUniformVec3("ptLight[0].color", ptLight->getColor());
		shader->setUniformFloat("ptLight[0].intensity", ptLight->getIntensity());
		s->renderWorldSpace(this);
	}
	ptLight->renderWorldSpace(this);
}

void GltfViewerScene::renderUi()
{
	for (Audace::Sprite* s : sprites)
	{
		s->renderViewSpace(this);
	}
}
