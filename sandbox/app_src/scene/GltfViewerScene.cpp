#include "GltfViewerScene.h"
#include "content/AssetStore.h"
#include "content/gltf/GltfLoader.h"
#include "renderer/ShaderProgram.h"
#include "renderer/Sprite.h"
#include "renderer/material/PbrMetalRoughMat.h"
#include "renderer/light/PointLight.h"
#include "scene/BaseCamera.h"
#include "scene/graph/SceneGraph.h"
#include "scene/graph/SceneGraphNode.h"
#include "scene/graph/RotationAnimation.h"
#include <vector>
using std::vector;

GltfViewerScene::GltfViewerScene(Audace::BaseAppController* controller)
	: Scene(controller)
{

}

void GltfViewerScene::loadAssets(Audace::IFileAccess* fileLoader)
{
	Audace::BaseMaterial* mat = Audace::AssetStore::simpleBillboardMaterial();

	Audace::GltfLoader loader;
	// std::string path = "models/quaternius/medieval_village/";
	// std::string filename = "Overhang_RoofIncline_UnevenBricks.gltf";
	// loader.setImageLoadPath("images/quaternius/");

	// std::string path = "models/quat_builds/";
	// std::string filename = "house_orig.gltf";
	// loader.setImageLoadPath("images/quaternius/");

	std::string path = "models/_test/";
	std::string filename = "Lantern.gltf";
	loader.setImageLoadPath("images/_test/");
	loader.loadFile(fileLoader, path, filename);
	sceneGraph = loader.getSceneGraph(this);

	anim = new Audace::RotationAnimation();
	vector<float> times = { 0.0, 0.25, 0.5, 0.75, 1.0 };
	vector<glm::quat> values = {
		{   0.0, 0.0,	0.0,     1.0},
		{ 0.707, 0.0,	0.0,   0.707},
		{   1.0, 0.0,	0.0,     0.0},
		{ 0.707, 0.0,	0.0,  -0.707},
		{   0.0, 0.0,	0.0,     1.0}
	};
	anim->setFrameTimes(times);
	anim->setFrameStates(values);
	anim->setTimeFactor(0.05);

	ptLight = new Audace::PointLight();
	ptLight->setColor({ 1, 1, 1 });
	ptLight->setIntensity(1);
	Audace::SceneGraphNode* n1 = new Audace::SceneGraphNode();
	n1->addAnimation(anim);
	n1->setTranslation({ 0, 0, 0 });
	lightNode = new Audace::SceneGraphNode(n1);
	lightNode->setTranslation({ 0, 3, 0 });
	lightNode->setSprite(ptLight);
	sceneGraph->addRootNode(n1);
	addSprite(ptLight);

	camera->setOriginPos({ 4, 6, 1 });

	glEnable(GL_CULL_FACE);
	glCullFace(GL_CW);
}

void GltfViewerScene::render()
{
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera->update();
	sceneGraph->update(this);
	glm::vec3 p = lightNode->getPosition();
	ptLight->setPosition(p);
	Audace::ShaderProgram* shader = Audace::AssetStore::getShader("pbr");
	shader->bind();
	shader->setUniformVec3("viewPos", camera->getPosition());
	shader->setUniformVec4("ambientLight", 1, 1, 1, 0.4);
	shader->setUniformVec3("ptLight[0].position", ptLight->getPosition());
	shader->setUniformVec3("ptLight[0].color", ptLight->getColor());
	shader->setUniformFloat("ptLight[0].intensity", ptLight->getIntensity());
	for (Audace::Sprite* s : sprites)
	{
		s->renderWorldSpace(this);
	}
}

void GltfViewerScene::renderUi()
{
	for (Audace::Sprite* s : sprites)
	{
		s->renderViewSpace(this);
	}
}

Audace::BaseCamera* GltfViewerScene::getCamera()
{
	return camera;
}

void GltfViewerScene::setCamera(Audace::BaseCamera* camera)
{
	this->camera = camera;
}

void GltfViewerScene::teleport(glm::vec3 pos)
{
	camera->setOriginPos(pos);
}
