#include <fstream>
#include <sstream>
#include "au_renderer.h"
#include "GltfViewerScene.h"
#include "imgui.h"
#include "SceneEnum.h"
#include "application/BaseAppController.h"
#include "content/IFileAccess.h"
#include "content/JsonSerializer.h"
#include "content/AssetStore.h"
#include "content/gltf/GltfLoader.h"
#include "content/gltf/GltfxReader.h"
#include "content/JsonGui.h"
#include "renderer/texture/Texture2d.h"
#include "renderer/texture/TextureCubemap.h"
#include "renderer/ShaderProgram.h"
#include "renderer/Mesh.h"
#include "renderer/Shapes.h"
#include "renderer/Sprite.h"
#include "renderer/light/PointLight.h"
#include "renderer/light/DirLight.h"
#include "renderer/light/SpotLight.h"
#include "renderer/light/TypedLight.h"
#include "renderer/texture/Texture2d.h"
#include "renderer/material/Material.h"
#include "renderer/FrameBuffer.h"
#include "scene/BaseCamera.h"
#include "scene/SceneDescriptor.h"
#include "scene/graph/SceneGraph.h"
#include "scene/graph/SceneGraphNode.h"
#include "util/StringUtil.h"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "editor/SceneEditor.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

#ifdef AU_PLATFORM_GLFW
#include "KeyboardManager.h"
#include "MouseManager.h"
#endif

enum RenderType
{
	POSITION,
	MTL_COLOR,
	NORMAL,
	AMBIENT,
	DIR_LIGHT,
	FULL
};

GltfViewerScene::GltfViewerScene(Audace::BaseAppController *controller)
	: Scene(controller)
{
	strcpy(sceneWritePath, "D:/audace_engine/sandbox/assets/scenes/MainScene.json");
}

void GltfViewerScene::loadAssets(Audace::IFileAccess *fileLoader)
{
	renderType = RenderType::FULL;
	this->fileLoader = fileLoader;
	sceneFilepath = "scenes/samples/sample_scenes.gltfx";

	// modelIndex = fileLoader->textFileToJson("models/_index.json");

	// framebuffer
	Audace::ImageData dat;
	dat.bytes = nullptr;
	dat.width = appController->getWidth();
	dat.height = appController->getHeight();
	dat.format = GL_RGB;
	Audace::Texture2d *fbTex = new Audace::Texture2d(dat);
	fbTex->create();
	frameBuffer = new Audace::FrameBuffer();
	frameBuffer->create();
	frameBuffer->colorAttachment(fbTex);
	frameBuffer->checkStatus();

	setAmbientLight({1, 1, 1, 0.4});
	shader = Audace::AssetStore::getShader("pbr");
	shader->bind();
	shader->setUniformInt("irradianceMap", 6);

	editor = new Audace::SceneEditor(fileLoader);
	editor->attachToScene(this);
	reloadScene();

	// TODO: put this info somewhere logical!
	// making cubemap texture from equirectangular image
	// use these angles (x, y, z) for the camera and render each face
	// nx: 0, 90, 0
	// ny: 0, 0, 0
	// nz: 270, 180, 0
	// px: 0, 270, 0
	// py: 0, 180, 0
	// pz: 90, 180, 0
	// make sure to use a square viewport size and 90deg fov on the camera
	Audace::Sprite *skyboxSprite = Audace::AssetStore::getCubeSprite();
	Audace::SimpleBillboardMaterial *mat = new Audace::SimpleBillboardMaterial();
	mat->setShader(Audace::AssetStore::getShader("skybox"));
	cubeTex = Audace::AssetStore::getCubemapTex(    "images/paris_skybox");
	cubeConvTex = Audace::AssetStore::getCubemapTex("images/paris_skybox/conv");

	mat->setTexture(cubeTex);
	skyboxSprite->getMesh()->setMaterial(mat);
	skyboxNode = new Audace::SceneGraphNode;
	skyboxNode->setName("skybox1");
	skyboxNode->setScale({400, 400, 400});
	skyboxNode->setSprite(skyboxSprite);
	sceneGraph->getRootNode()->addChild(skyboxNode);
}

void GltfViewerScene::render()
{
	frameBuffer->bind();
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera->update();
	skyboxNode->setTranslation(camera->getPosition());
	sceneGraph->update(this);

	shader->bind();
	shader->setUniformVec3("viewPos", camera->getPosition());
	shader->setUniformVec4("ambientLight", ambientColor);
	cubeConvTex->bind(6);

	for (auto &item : lights)
	{
		shader->setUniformLight(item.second);
	}

	editor->renderWorldSpace(this);

	for (Audace::Sprite *s : sprites)
	{
		s->renderWorldSpace(this);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1, 0, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render the framebuffer texture onto a quad
	{
		Audace::SimpleBillboardMaterial *mat = new Audace::SimpleBillboardMaterial();
		mat->setTexture(frameBuffer->getColorTexAttachment());
		mat->setColor({1, 1, 1, 0});
		Audace::ShaderProgram *s = Audace::AssetStore::getShader("AU_post_proc");
		s->bind();
		s->setUniformMat4("vpMat", glm::mat4(1));
		mat->setShader(s);
		Audace::Mesh *m = Audace::Shapes::squarePositions();
		glm::mat4 worldMat = glm::mat4(1);
		worldMat = glm::translate(worldMat, {-1, -1, 0});
		worldMat = glm::scale(worldMat, {2, 2, 2});
		m->setMaterial(mat);
		m->renderInstanced({worldMat});
	}
}

void GltfViewerScene::reloadScene()
{
	if (sceneGraph != nullptr)
	{
		sceneGraph->dispose();
		delete sceneGraph;
	}
	Audace::GltfxReader reader(fileLoader);
	sceneGraph = new Audace::SceneGraph;
	Audace::SceneGraphNode *root = reader.readDefaultScene(sceneFilepath);
	sceneGraph->setRootNode(root);
	editor->setSceneGraph(sceneGraph);
}

void GltfViewerScene::loadModel(std::string path, std::string filename)
{
}

void GltfViewerScene::setClearColor(glm::vec4 color)
{
	clearColor = color;
}

void GltfViewerScene::setAmbientLight(glm::vec4 color)
{
	ambientColor = color;
}

void GltfViewerScene::setLight(Audace::LightType type, Audace::Sprite *sprite)
{
	const std::string &name = sprite->getName();
	switch (type)
	{
	case Audace::LightType::POINT_LIGHT:
	{
		Audace::PointLight *ptLight = reinterpret_cast<Audace::PointLight *>(sprite);
		if (lights.find(name) == lights.end())
		{
			// the light entry doesn't exist yet
			Audace::TypedLight *light = new Audace::TypedLight(name, ptLight);
			lights[name] = light;
		}
		else
		{
			lights[name]->ptLight->setColor(ptLight->getColor());
			lights[name]->ptLight->setIntensity(ptLight->getIntensity());
		}
	}
	break;

	case Audace::LightType::DIRECTIONAL_LIGHT:
	{
		Audace::DirLight *dirLight = reinterpret_cast<Audace::DirLight *>(sprite);
		if (lights.find(name) == lights.end())
		{
			// the light entry doesn't exist yet
			Audace::TypedLight *light = new Audace::TypedLight(name, dirLight);
			lights[name] = light;
		}
		else
		{
			lights[name]->dirLight->setColor(dirLight->getColor());
			lights[name]->dirLight->setIntensity(dirLight->getIntensity());
		}
	}
	break;

	case Audace::LightType::SPOTLIGHT:
	{
		Audace::SpotLight *spotLight = reinterpret_cast<Audace::SpotLight *>(sprite);
		if (lights.find(name) == lights.end())
		{
			// the light entry doesn't exist yet
			Audace::TypedLight *light = new Audace::TypedLight(name, spotLight);
			lights[name] = light;
		}
		else
		{
			lights[name]->spotLight->setColor(spotLight->getColor());
			lights[name]->spotLight->setIntensity(spotLight->getIntensity());
			lights[name]->spotLight->setInnerAngle(spotLight->getInnerAngle());
			lights[name]->spotLight->setOuterAngle(spotLight->getOuterAngle());
		}
	}
	break;
	}
}

Audace::TypedLight *GltfViewerScene::getLight(const std::string &name)
{
	return lights[name];
}

void GltfViewerScene::setCamera(Audace::BaseCamera *camera)
{
	this->camera = camera;
}

Audace::BaseCamera *GltfViewerScene::getCamera()
{
	return camera;
}

void GltfViewerScene::teleport(glm::vec3 pos)
{
	camera->setOriginPos(pos);
}

void GltfViewerScene::renderUi()
{
	// if (currSprite != nullptr)
	// {
	// 	editWin->renderViewSpace(this);
	// }
	editor->sceneEditWindow();
}

void GltfViewerScene::disposeAssets()
{
}