#include <fstream>
#include <sstream>
#include "au_renderer.h"
#include "SceneBuilder.h"
#include "imgui.h"
#include "SceneEnum.h"
#include "content/JsonSerializer.h"
#include "content/AssetStore.h"
#include "renderer/ShaderProgram.h"
#include "renderer/Shapes.h"
#include "renderer/material/Material.h"
#include "util/StringUtil.h"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"

#ifdef AU_PLATFORM_GLFW
#include "KeyboardManager.h"
#include "MouseManager.h"
#endif

void SceneBuilder::loadAssets(Audace::FileLoader *fileLoader)
{
	this->fileLoader = fileLoader;
	Audace::ShaderProgram *shader = Audace::AssetStore::getShader("obj_mtl");
	shader->create();
	shader->setUniformVec3("textureScale", {10, 10, 10});

	quadMesh = Audace::Shapes::squarePositions();

	modelMat = glm::rotate(modelMat, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	modelBasePath = "models/";
	modelFiles = fileLoader->listFilesInDir(modelBasePath + "*.obj");
	modelCount = modelFiles.size();
	currSprite = loadModel(modelFiles[selectedModelIndex]);
	currSprite->setModelMatrix(modelMat);

	{
		startPosMarker = new Audace::Sprite({Audace::Shapes::spherePositions(8, 8)});
		Audace::SimpleBillboardMaterial *mat = Audace::AssetStore::simpleBillboardMaterial();
		mat->setTexture(Audace::AssetStore::getWhiteTexture());
		startPosMarker->getMesh()->setMaterial(mat);
		startPosMarker->setScale({0.1f, 0.1f, 0.1f});
	}
}

Audace::Sprite *SceneBuilder::loadModel(std::string modelName)
{
	Audace::Sprite *model = Audace::AssetStore::cloneSprite(modelName);
	Audace::ShaderProgram *shader = Audace::AssetStore::getShader("obj_mtl");
	model->forEachMesh([this, shader](Audace::Mesh *mesh)
					   {
		Audace::Material *material = reinterpret_cast<Audace::Material *>(mesh->getMaterial());

		// set up a valid default material before replacing it
		material->setShader(shader);
		bool found = false;
		for (auto &item : matRecs)
		{
			if (Audace::StringUtil::startsWith(material->getName(), item.first))
			{
				mesh->setMaterial(item.second.material);
				found = true;
			}
		} 
		if (!found)
		{
			matRecs[material->getName()] = MaterialRec(material);
		}
	});
	currMaterial = (*matRecs.begin()).second.name;
	return model;
}

void SceneBuilder::newScene()
{
	while (!sprites.empty())
	{
		Audace::Sprite *s = sprites.back();
		delete s;
		sprites.pop_back();
	}

	jsonContent = {};
}

void SceneBuilder::saveScene(std::string filename)
{
	jsonContent["startPos"] = {startPos.x, startPos.y, startPos.z};
	std::ofstream fout(filename, std::ios::out);
	std::string s = jsonContent.dump(4);
	fout.write(s.c_str(), s.length());
	fout.close();
}

void SceneBuilder::loadScene(std::string filename)
{
	newScene();

	std::ifstream fin(filename, std::ios::in);
	std::stringstream ss;
	ss << fin.rdbuf();
	jsonContent = json::parse(ss.str());
	startPos = {jsonContent["startPos"][0], jsonContent["startPos"][1], jsonContent["startPos"][2]};

	glm::mat4 IDENTITY_MAT = glm::mat4(1.0f);
	glm::mat4 modelMat = glm::rotate(glm::scale(IDENTITY_MAT, {1, 1, 1}), glm::radians(90.0f), glm::vec3(1, 0, 0));

	for (auto &item : jsonContent["materials"].items())
	{
		std::string name = item.key();
		std::string path = item.value();
		Audace::Material *mat = Audace::JsonSerializer::loadMaterial(fileLoader->textFileToJson(path));
		MaterialRec rec;
		strcpy(rec.name, mat->getName().c_str());
		rec.ambientColor = mat->getAmbientColor();
		rec.diffuseColor = mat->getDiffuseColor();
		rec.specularColor = mat->getSpecularColor();
		rec.emissionColor = mat->getEmissionColor();
		rec.material = mat;
		matRecs[name] = rec;
	}
	currMaterial = (*matRecs.begin()).second.name;

	for (auto &item : jsonContent["sprites"].items())
	{
		std::string filename = item.key();
		json list = item.value();
		for (auto &it : list.items())
		{
			json obj = it.value();
			json pos = obj["position"];
			json angles = obj["orientation"];
			Audace::Sprite *sprite = loadModel(filename);
			sprite->setModelMatrix(modelMat);
			sprite->setPosition({pos[0], pos[1], pos[2]});
			sprite->setOrientation(glm::quat(glm::radians(glm::vec3(angles[0], angles[1], angles[2]))));
			sprites.push_back(sprite);
		}
	}

	pointLights[0].setPosition({0, 0, 5});
	sprites.push_back(&pointLights[0]);
}

void SceneBuilder::render()
{
	glClearColor(clearColor.x, clearColor.y, clearColor.z, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera->update();

	Audace::ShaderProgram *shader = Audace::AssetStore::getShader("obj_mtl");
	shader->bind();
	shader->setUniformVec4("ambientLight", 1, 1, 1, 1);

	for (int i = 0; i < 1; i++)
	{
		std::string prefix = "light[";
		prefix += '0' + i;
		prefix += "]";
		shader->setUniformVec3(prefix + ".position", pointLights[i].getPosition());
		shader->setUniformVec3(prefix + ".color", pointLights[i].getColor());
		shader->setUniformFloat(prefix + ".intensity", pointLights[i].getIntensity());
	}

	shader->setUniformVec3("viewPos", camera->getPosition());

	for (Audace::Sprite *sprite : sprites)
	{
		sprite->render(this);
	}

	currSprite->setPosition(spritePos);
	currSprite->setOrientation(glm::quat(glm::radians(spriteAngles)));
	currSprite->setScale(spriteScale);
	currSprite->render(this);

	startPosMarker->setPosition(pointLights[0].getPosition());
	startPosMarker->render(this);

	// ImGui::ShowDemoWindow();
	ImGui::Begin("Scenes");
	if (ImGui::Button("Navigation"))
	{
		appController->setScene(SandboxScene::NAVIGATION);
	}
	if (ImGui::Button("Main"))
	{
		appController->setScene(SandboxScene::MAIN);
	}
	ImGui::End();

	ImGui::Begin("Scene Editor");
	if (ImGui::BeginTabBar("Tools"))
	{
		if (ImGui::BeginTabItem("Scene"))
		{
			ImGui::ColorEdit3("Clear Color", glm::value_ptr(clearColor));
			ImGui::DragFloat3("Start position", glm::value_ptr(startPos), 0.1f);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Models"))
		{
			if (ImGui::BeginCombo("Filename", modelFiles[selectedModelIndex].c_str()))
			{
				for (int i = 0; i < modelCount; i++)
				{
					bool selected = selectedModelIndex == i;
					if (ImGui::Selectable(modelFiles[i].c_str(), selected))
					{
						selectedModelIndex = i;
						currSprite = loadModel(modelFiles[i]);
						currSprite->setModelMatrix(modelMat);
					}
					if (selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
			if (ImGui::Button("Add"))
			{
				json obj = {
					{"position", {spritePos.x, spritePos.y, spritePos.z}},
					{"orientation", {spriteAngles.x, spriteAngles.y, spriteAngles.z}}};
				jsonContent["sprites"][modelFiles[selectedModelIndex]].push_back(obj);

				currSprite->setPosition(spritePos);
				currSprite->setOrientation(glm::quat(glm::radians(spriteAngles)));
				currSprite->setScale(spriteScale);
				sprites.push_back(currSprite);
				currSprite = currSprite->clone();
				currSprite->setModelMatrix(modelMat);
			}
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Materials"))
		{

			if (ImGui::BeginCombo("Material", currMaterial))
			{
				for (auto &item : matRecs)
				{
					MaterialRec &rec = item.second;
					if (ImGui::Selectable(rec.name, currMaterial == rec.name))
					{
						currMaterial = rec.name;
					}
				}
				ImGui::EndCombo();
			}

			MaterialRec &rec = matRecs[std::string(currMaterial)];
			ImGui::ColorEdit3("Ambient##material", glm::value_ptr(rec.ambientColor));
			ImGui::ColorEdit3("Diffuse##material", glm::value_ptr(rec.diffuseColor));
			ImGui::ColorEdit3("Specular##material", glm::value_ptr(rec.specularColor));
			ImGui::ColorEdit3("Emission##material", glm::value_ptr(rec.emissionColor));
			// ImGui::InputText("Ambient Occlusion Map", rec.aoMap, 256);
			// ImGui::InputText("Diffuse Map", rec.diffuseMap, 256);
			// ImGui::InputText("Normal Map", rec.normalMap, 256);
			// ImGui::InputText("Roughness Map", rec.roughnessMap, 256);
			rec.material->setAmbientColor(rec.ambientColor);
			rec.material->setDiffuseColor(rec.diffuseColor);
			rec.material->setSpecularColor(rec.specularColor);
			rec.material->setEmissionColor(rec.emissionColor);
			// rec.material->setAmbientOcclusionMap(Audace::AssetStore::getTexture(rec.aoMap));
			// rec.material->setDiffuseMap(Audace::AssetStore::getTexture(rec.diffuseMap));
			// rec.material->setNormalMap(Audace::AssetStore::getTexture(rec.normalMap));
			// rec.material->setSpecularMap(Audace::AssetStore::getTexture(rec.roughnessMap));
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Sprite Properties"))
		{
			if (ImGui::BeginTable("World Tx", 4, 0))
			{
				ImGui::TableSetupColumn("Tx", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("x", ImGuiTableColumnFlags_WidthStretch, 150);
				ImGui::TableSetupColumn("y", ImGuiTableColumnFlags_WidthFixed, 150);
				ImGui::TableSetupColumn("z", ImGuiTableColumnFlags_WidthFixed, 150);
				ImGui::TableNextColumn();
				ImGui::Text("Position");
				ImGui::TableNextColumn();
				ImGui::DragFloat("posx", &spritePos.x, 0.05f);
				if (ImGui::Button("-##posx"))
				{
					spritePos.x--;
				}
				ImGui::SameLine();
				if (ImGui::Button("+##posx"))
				{
					spritePos.x++;
				}
				ImGui::TableNextColumn();
				ImGui::DragFloat("posy", &spritePos.y, 0.05f);
				if (ImGui::Button("-##posy"))
				{
					spritePos.y--;
				}
				ImGui::SameLine();
				if (ImGui::Button("+##posy"))
				{
					spritePos.y++;
				}
				ImGui::TableNextColumn();
				ImGui::DragFloat("posz", &spritePos.z, 0.05f);
				if (ImGui::Button("-##posz"))
				{
					spritePos.z--;
				}
				ImGui::SameLine();
				if (ImGui::Button("+##posz"))
				{
					spritePos.z++;
				}

				ImGui::TableNextColumn();
				ImGui::Text("Angles");
				ImGui::TableNextColumn();
				ImGui::DragFloat("th-x", &spriteAngles.x, 0.05f);
				if (ImGui::Button("-##th-x"))
				{
					spriteAngles.x--;
				}
				ImGui::SameLine();
				if (ImGui::Button("+##th-x"))
				{
					spriteAngles.x++;
				}
				ImGui::TableNextColumn();
				ImGui::DragFloat("th-y", &spriteAngles.y, 0.05f);
				if (ImGui::Button("-##th-y"))
				{
					spriteAngles.y--;
				}
				ImGui::SameLine();
				if (ImGui::Button("+##th-y"))
				{
					spriteAngles.y++;
				}
				ImGui::TableNextColumn();
				ImGui::DragFloat("th-z", &spriteAngles.z, 0.05f);
				if (ImGui::Button("-##th-z"))
				{
					spriteAngles.z--;
				}
				ImGui::SameLine();
				if (ImGui::Button("+##th-z"))
				{
					spriteAngles.z++;
				}

				ImGui::TableNextColumn();
				ImGui::Text("Scale");
				ImGui::TableNextColumn();
				ImGui::DragFloat("scale x", &spriteScale.x, 0.05f);
				if (ImGui::Button("-##scalex"))
				{
					spriteScale.x--;
				}
				ImGui::SameLine();
				if (ImGui::Button("+##scalex"))
				{
					spriteScale.x++;
				}
				ImGui::TableNextColumn();
				ImGui::DragFloat("scale y", &spriteScale.y, 0.05f);
				if (ImGui::Button("-##scaley"))
				{
					spriteScale.y--;
				}
				ImGui::SameLine();
				if (ImGui::Button("+##scaley"))
				{
					spriteScale.y++;
				}
				ImGui::TableNextColumn();
				ImGui::DragFloat("scale z", &spriteScale.z, 0.05f);
				if (ImGui::Button("-##scalez"))
				{
					spriteScale.z--;
				}
				ImGui::SameLine();
				if (ImGui::Button("+##scalez"))
				{
					spriteScale.z++;
				}

				ImGui::EndTable();
			}
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Lights"))
		{
			static int currLight = 0;
			ImGui::RadioButton("1", &currLight, 0);
			ImGui::SameLine();
			ImGui::RadioButton("2", &currLight, 1);
			ImGui::SameLine();
			ImGui::RadioButton("3", &currLight, 2);
			ImGui::SameLine();
			ImGui::RadioButton("4", &currLight, 3);

			static glm::vec3 position;
			static glm::vec3 color;
			static float intensity;
			if (ImGui::DragFloat3("Position", glm::value_ptr(position), 0.01f))
			{
				pointLights[currLight].setPosition(position);
			}
			if (ImGui::ColorEdit3("Color", glm::value_ptr(color)))
			{
				pointLights[currLight].setColor(color);
			}
			if (ImGui::SliderFloat("Intensity", &intensity, 0.0f, 1.5f))
			{
				pointLights[currLight].setIntensity(intensity);
			}
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("File"))
		{
			ImGui::InputText("File path", sceneWritePath, scenePathLength);

			if (ImGui::Button("New"))
			{
				newScene();
			}
			ImGui::SameLine();
			if (ImGui::Button("Save"))
			{
				saveScene(std::string(sceneWritePath));
			}
			ImGui::SameLine();
			if (ImGui::Button("Load"))
			{
				loadScene(std::string(sceneWritePath));
			}
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
	ImGui::End();
	// ImGui::Begin("Lights");
	// if (ImGui::BeginTabBar("Light number", 0))
	// {
	// 	for (int i = 0; i < 4; i++)
	// 	{
	// 		std::string name = (std::string("Light ") + std::to_string(i));
	// 		if (ImGui::BeginTabItem(name.c_str()))
	// 		{
	// 			ImGui::DragFloat3((std::string("Position") + std::to_string(i)).c_str(), glm::value_ptr(pointLights[i].position));
	// 			ImGui::ColorEdit3((std::string("Color") + std::to_string(i)).c_str(), glm::value_ptr(pointLights[i].color));
	// 			ImGui::DragFloat((std::string("Intensity") + std::to_string(i)).c_str(), &pointLights[i].intensity, 0.1f, 0.0f, 1.5f);
	// 			ImGui::EndTabItem();
	// 		}
	// 	}
	// 	ImGui::EndTabBar();
	// }
	// ImGui::End();
}

void SceneBuilder::disposeAssets()
{
}