#include <sstream>
#include <vector>
#include <map>
#include "AuLogger.h"
#include "ModelLoader.h"
#include "util/StringUtil.h"
#include "glm/glm.hpp"

namespace Audace
{
	Model *ModelLoader::loadObj(std::string &fileContent)
	{
		Model *model = new Model;
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec3> texCoords;
		std::map<std::string, unsigned int> indexMap;

		unsigned int currentIndex = 0;
		ModelSection* currentSection = nullptr;

		std::stringstream ss(fileContent);
		std::string line;
		std::string d = "\n";
		while (std::getline(ss, line, '\n'))
		{
			if (line[0] == '#')
			{
				// ignore comment
			}
			else if (line.size() == 0)
			{
				// ignore blank line
			}
			else if (line[0] == 'u' && line[1] == 's' && line[2] == 'e' && line[3] == 'm' && line[4] == 't' && line[5] == 'l')
			{
				if (currentSection != nullptr)
				{
					model->sections.push_back(currentSection);
				}
				currentSection = new ModelSection;
				currentSection->startIndex = currentIndex;
				currentSection->vertexCount = 0;
			}
			else if (line[0] == 'g')
			{
				model->name = line.substr(line.find(' ', 0) + 1);
			}
			else if (line[0] == 'v' && line[1] == 'n')
			{
				// vertex normal
				std::vector<std::string> vec = StringUtil::split(line, ' ');
				glm::vec3 pos = glm::vec3(std::stof(vec[1]), std::stof(vec[2]), std::stof(vec[3]));
				normals.push_back(pos);
			}
			else if (line[0] == 'v' && line[1] == 't')
			{
				// tex coord
				std::vector<std::string> vec = StringUtil::split(line, ' ');
				glm::vec3 pos = glm::vec3(std::stof(vec[1]), std::stof(vec[2]), 0);
				texCoords.push_back(pos);
			}
			else if (line[0] == 'v')
			{
				// vertex position
				std::vector<std::string> vec = StringUtil::split(line, ' ');
				glm::vec3 pos = glm::vec3(std::stof(vec[1]), std::stof(vec[2]), std::stof(vec[3]));
				positions.push_back(pos);
			}
			else if (line[0] == 'f')
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
						model->indices.push_back(index);
						currentSection->vertexCount++;
						currentIndex++;
					}
					else
					{
						// this vertex already exists, just add the index
						model->indices.push_back(indexMap[face]);
						currentSection->vertexCount++;
						currentIndex++;
					}
				}
			}
			else
			{
				AU_ENGINE_LOG_DEBUG("Read line: {}", line);
			}
		}
		model->sections.push_back(currentSection);
		return model;
	}
}