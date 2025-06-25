#include "GltfViewerScene.h"
#include "content/AssetStore.h"
#include "content/gltf/GltfLoader.h"
#include "renderer/material/PbrMetalRoughMat.h"

void GltfViewerScene::loadAssets(Audace::IFileAccess* fileLoader)
{
	Audace::BaseMaterial* mat = Audace::AssetStore::simpleBillboardMaterial();

	Audace::GltfLoader loader;
	// std::string path = "models/quaternius/medieval_village/";
	// std::string filename = "Overhang_RoofIncline_UnevenBricks.gltf";
	std::string path = "models/_test/";
	std::string filename = "gold_triangle.gltf";
	loader.loadFile(fileLoader, path, filename);
	sceneGraph = loader.getSceneGraph(this);
	ptLight = new Audace::PointLight();
	ptLight->setColor({ 1, 1, 1 });
	ptLight->setIntensity(1);
	ptLight->setPosition({ 0.5, 0.5, 5 });
	Audace::Texture2d* normTex = Audace::AssetStore::getTexture("images/quaternius/T_Plaster_Normal.png");
	Audace::Texture2d* metalRoughTex = Audace::AssetStore::getTexture("images/quaternius/T_Plaster_ORM.png");
	Audace::PbrMetalRoughMat* pbrMat = reinterpret_cast<Audace::PbrMetalRoughMat*>(sprites[0]->getMaterial());
	pbrMat->setNormalMap(normTex);
	pbrMat->setMetallicMap(metalRoughTex);
	pbrMat->setRoughnessMap(metalRoughTex);
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
		shader->setUniformVec3("viewPos", camera->getPosition());
		shader->setUniformVec3("ptLight[0].position", ptLight->getPosition());
		shader->setUniformVec3("ptLight[0].color", ptLight->getColor());
		shader->setUniformFloat("ptLight[0].intensity", ptLight->getIntensity());
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
