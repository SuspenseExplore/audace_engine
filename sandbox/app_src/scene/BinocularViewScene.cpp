#include <fstream>
#include <sstream>
#include "au_renderer.h"
#include "BinocularViewScene.h"
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
#include "scene/ForwardCamera.h"
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

BinocularViewScene::BinocularViewScene(Audace::BaseAppController *controller)
	: Scene(controller)
{
	strcpy(sceneWritePath, "D:/audace_engine/sandbox/assets/scenes/MainScene.json");
}

void BinocularViewScene::loadAssets(Audace::IFileAccess *fileLoader)
{
	renderType = RenderType::FULL;
	this->fileLoader = fileLoader;
	sceneFilepath = "scenes/samples/sample_scenes.gltfx";

	{
		float v[9] = {0, 0, 0, 0, 1, 0, 0, 0, 0};
		Audace::ShaderProgram *s = Audace::AssetStore::getShader("AU_post_proc");
		s->bind();
		s->setUniformFloatArray("kernel[0]", v, 9);
		s->setUniformFloat("offsetScale", 1);
	}

	// framebuffer
	Audace::ImageData dat;
	dat.bytes = nullptr;
	dat.width = appController->getWidth() / 2;
	dat.height = appController->getHeight();
	dat.format = GL_RGB;
	for (int i = 0; i < 2; i++)
	{
		Audace::Texture2d *fbTex = new Audace::Texture2d(dat);
		fbTex->create();
		frameBuffer[i] = new Audace::FrameBuffer();
		frameBuffer[i]->create();
		frameBuffer[i]->colorAttachment(fbTex);
		frameBuffer[i]->checkStatus();
	}

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
	cubeTex = Audace::AssetStore::getCubemapTex("images/milkyway_skybox");
	cubeConvTex = Audace::AssetStore::getCubemapTex("images/milkyway_skybox/conv");

	mat->setTexture(cubeTex);
	skyboxSprite->getMesh()->setMaterial(mat);
	skyboxNode = new Audace::SceneGraphNode;
	skyboxNode->setName("skybox1");
	skyboxNode->setScale({400, 400, 400});
	skyboxNode->setSprite(skyboxSprite);
	sceneGraph->getRootNode()->addChild(skyboxNode);
}

void BinocularViewScene::render()
{
	camera->update();
	Audace::ForwardCamera *origCam = reinterpret_cast<Audace::ForwardCamera *>(camera);
	Audace::ForwardCamera *eyeCam[2];
	eyeCam[0] = new Audace::ForwardCamera(origCam->getPose(), origCam->getProjMat());
	eyeCam[1] = new Audace::ForwardCamera(origCam->getPose(), origCam->getProjMat());
	glViewport(0, 0, appController->getWidth() / 2, appController->getHeight());

	static float offsetVal = 0.03;
	static float angleVal = 0.001;
	static bool crossView = false;
	float offset[2] = {-offsetVal, offsetVal};
	float angle[2] = {-angleVal, angleVal};
	float lr[2] = {0, -1};

	for (int i = 0; i < 2; i++)
	{
		eyeCam[i]->move(origCam->getRightVec() * offset[i]);
		eyeCam[i]->rotate(0, 0, angle[i]);
		camera = eyeCam[i];
		frameBuffer[i]->bind();
		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
	}
	camera = origCam;

	glBindFramebuffer(GL_FRAMEBUFFER, appController->getDefaultFramebuffer());
	glViewport(0, 0, appController->getWidth(), appController->getHeight());
	glClearColor(1, 0, 1, 0.5);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render the framebuffers side by side
	{
		static Audace::SimpleBillboardMaterial *mat = new Audace::SimpleBillboardMaterial();
		mat->setColor({1, 1, 1, 0});
		Audace::ShaderProgram *s = Audace::AssetStore::getShader("AU_post_proc");
		s->bind();
		s->setUniformMat4("vpMat", glm::mat4(1));
		mat->setShader(s);
		Audace::Mesh *m = Audace::Shapes::squarePositions();
		m->setMaterial(mat);

		{
			glm::mat4 worldMat = glm::mat4(1);
			worldMat = glm::translate(worldMat, {crossView ? lr[0] : lr[1], -1, 0});
			worldMat = glm::scale(worldMat, {1, 2, 1});
			mat->setTexture(frameBuffer[0]->getColorTexAttachment());
			m->renderInstanced({worldMat});
		}
		{
			glm::mat4 worldMat = glm::mat4(1);
			worldMat = glm::translate(worldMat, {crossView ? lr[1] : lr[0], -1, 0});
			worldMat = glm::scale(worldMat, {1, 2, 1});
			mat->setTexture(frameBuffer[1]->getColorTexAttachment());
			m->renderInstanced({worldMat});
		}
	}

	ImGui::Begin("BinocularView");
	ImGui::DragFloat("offset", &offsetVal, 0.001, 0, 1);
	ImGui::DragFloat("angle", &angleVal, 0.0001, 0, 1, "%.6f");
	ImGui::Checkbox("Cross view", &crossView);
	ImGui::End();
}

void BinocularViewScene::reloadScene()
{
	if (sceneGraph != nullptr)
	{
		sceneGraph->dispose();
		delete sceneGraph;
	}
	Audace::GltfxReader reader(fileLoader);
	sceneGraph = new Audace::SceneGraph;
	Audace::SceneGraphNode *root = reader.readScene(sceneFilepath, 0);
	sceneGraph->setRootNode(root);
	{
		Audace::SceneGraphNode *r = reader.readScene(sceneFilepath, 1);
		r->setTranslation({5, 0, 1});
		r->setScale({4, 4, 4});
		root->addChild(r);
	}
	editor->setSceneGraph(sceneGraph);
}

void BinocularViewScene::loadModel(std::string path, std::string filename)
{
}

void BinocularViewScene::setClearColor(glm::vec4 color)
{
	clearColor = color;
}

void BinocularViewScene::setAmbientLight(glm::vec4 color)
{
	ambientColor = color;
}

void BinocularViewScene::setLight(Audace::LightType type, Audace::Sprite *sprite)
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

Audace::TypedLight *BinocularViewScene::getLight(const std::string &name)
{
	return lights[name];
}

void BinocularViewScene::setCamera(Audace::BaseCamera *camera)
{
	this->camera = camera;
}

Audace::BaseCamera *BinocularViewScene::getCamera()
{
	return camera;
}

void BinocularViewScene::teleport(glm::vec3 pos)
{
	camera->setOriginPos(pos);
}

void BinocularViewScene::renderUi()
{
	// if (currSprite != nullptr)
	// {
	// 	editWin->renderViewSpace(this);
	// }
	editor->sceneEditWindow();
}

void BinocularViewScene::disposeAssets()
{
}