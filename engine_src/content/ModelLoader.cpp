#include <sstream>
#include <vector>
#include <map>
#include "AuLogger.h"
#include "ModelLoader.h"
#include "util/StringUtil.h"
#include "glm/glm.hpp"

namespace Audace
{
	Model *ModelLoader::loadObj(FileLoader *fileLoader, std::string path, std::string filename)
	{
		Model *model = new Model;
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec3> texCoords;
		std::map<std::string, unsigned int> indexMap;

		unsigned int currentIndex = 0;
		ModelSection *currentSection = nullptr;
		std::map<std::string, Material *> materials;

		std::string fileContent = fileLoader->textFileToString(path + filename);
		std::stringstream ss(fileContent);
		std::string line;
		while (std::getline(ss, line))
		{
			line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
			if (StringUtil::startsWith(line, "#"))
			{
				// ignore comment
			}
			else if (line.empty())
			{
				// ignore blank line
			}
			else if (StringUtil::startsWith(line, "usemtl"))
			{
				std::vector<std::string> vec = StringUtil::split(line, ' ');
				std::string matName = vec[1];

				if (currentSection != nullptr)
				{
					model->sections.push_back(currentSection);
				}
				currentSection = new ModelSection;
				currentSection->material = materials[matName];
			}
			else if (StringUtil::startsWith(line, "g"))
			{
				model->name = line.substr(line.find(' ', 0) + 1);
			}
			else if (StringUtil::startsWith(line, "vn"))
			{
				// vertex normal
				std::vector<std::string> vec = StringUtil::split(line, ' ');
				glm::vec3 pos = glm::vec3(std::stof(vec[1]), std::stof(vec[2]), std::stof(vec[3]));
				normals.push_back(pos);
			}
			else if (StringUtil::startsWith(line, "vt"))
			{
				// tex coord
				std::vector<std::string> vec = StringUtil::split(line, ' ');
				glm::vec3 pos = glm::vec3(std::stof(vec[1]), std::stof(vec[2]), 0);
				texCoords.push_back(pos);
			}
			else if (StringUtil::startsWith(line, "v"))
			{
				// vertex position
				std::vector<std::string> vec = StringUtil::split(line, ' ');
				glm::vec3 pos = glm::vec3(std::stof(vec[1]), std::stof(vec[2]), std::stof(vec[3]));
				positions.push_back(pos);
			}
			else if (StringUtil::startsWith(line, "f"))
			{
				// face
				std::vector<std::string> vec = StringUtil::split(line, ' ');
				for (int i = 1; i < 4; i++)
				{
					std::string face = vec[i];
					auto iter = indexMap.find(face);
					if (iter == indexMap.end())
					{
						// this vertex isn't in the map yet
						Vertex vertex;
						std::vector<std::string> refs = StringUtil::split(face, '/');
						vertex.position = positions[std::stoi(refs[0]) - 1];
						vertex.texCoord = texCoords[std::stoi(refs[1]) - 1];
						vertex.normal = normals[std::stoi(refs[2]) - 1];

						unsigned int index = model->vertices.size();
						indexMap[face] = index;
						model->vertices.push_back(vertex);
						currentSection->indices.push_back(index);
						currentIndex++;
					}
					else
					{
						// this vertex already exists, just add the index
						currentSection->indices.push_back(indexMap[face]);
						currentIndex++;
					}
				}
			}
			else if (StringUtil::startsWith(line, "mtllib"))
			{
				std::vector<std::string> vec = StringUtil::split(line, ' ');
				materials = loadMtl(fileLoader, path, vec[1]);
			}
			else
			{
				// AU_ENGINE_LOG_DEBUG("Read line: '{}'", line);
			}
		}
		model->sections.push_back(currentSection);
		return model;
	}

	std::map<std::string, Material *> ModelLoader::loadMtl(FileLoader *fileLoader, std::string path, std::string filename)
	{
		std::map<std::string, Material *> mats;
		std::string fileContent = fileLoader->textFileToString(path + filename);
		std::stringstream ss(fileContent);
		std::string line;

		Material *mat = nullptr;
		bool foundAmbientColor = false;
		glm::vec3 diffuseColor;

		while (std::getline(ss, line, '\n'))
		{
			line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
			if (line[0] == '#')
			{
				// ignore comment
			}
			else if (line.size() == 0)
			{
				// ignore blank line
			}
			else if (line[0] == 'n' && line[1] == 'e' && line[2] == 'w' && line[3] == 'm' && line[4] == 't' && line[5] == 'l')
			{
				std::vector<std::string> vec = StringUtil::split(line, ' ');
				std::string name = vec[1];
				if (mat != nullptr)
				{
					if (!foundAmbientColor)
					{
						mat->setAmbientColor(diffuseColor);
					}
					mats[mat->getName()] = mat;
				}
				mat = new Material();
				mat->setName(name);
				foundAmbientColor = false;
			}
			else if (line[0] == 'K' && line[1] == 'a')
			{
				std::vector<std::string> vec = StringUtil::split(line, ' ');
				glm::vec3 ambient = glm::vec3(std::stof(vec[1]), std::stof(vec[2]), std::stof(vec[3]));
				mat->setAmbientColor(ambient);
				foundAmbientColor = true;
			}
			else if (line[0] == 'K' && line[1] == 'd')
			{
				std::vector<std::string> vec = StringUtil::split(line, ' ');
				diffuseColor = glm::vec3(std::stof(vec[1]), std::stof(vec[2]), std::stof(vec[3]));
				mat->setDiffuseColor(diffuseColor);
			}
			else if (line[0] == 'K' && line[1] == 's')
			{
				std::vector<std::string> vec = StringUtil::split(line, ' ');
				glm::vec3 specular = glm::vec3(std::stof(vec[1]), std::stof(vec[2]), std::stof(vec[3]));
				mat->setSpecularColor(specular);
			}
		}
		if (!foundAmbientColor)
		{
			mat->setAmbientColor(diffuseColor);
		}
		mats[mat->getName()] = mat;

		return mats;
	}
}