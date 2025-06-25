#include "GltfViewerScene.h"
#include "content/AssetStore.h"
#include "content/gltf/GltfLoader.h"

void GltfViewerScene::loadAssets(Audace::IFileAccess* fileLoader)
{
	Audace::BaseMaterial* mat = Audace::AssetStore::simpleBillboardMaterial();

	Audace::GltfLoader loader;
	loader.loadFile(fileLoader, "models/quaternius/medieval_village/", "Overhang_RoofIncline_UnevenBricks.gltf");
	sceneGraph = loader.getSceneGraph(this);
	for (Audace::Sprite* s : sprites)
	{
		s->forEachMesh([=](Audace::Mesh* m)
			{
				m->setMaterial(mat);
			});
	}
}

void GltfViewerScene::render()
{
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera->update();
	sceneGraph->update(this);
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
