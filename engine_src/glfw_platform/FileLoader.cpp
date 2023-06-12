#include <fstream>
#include <sstream>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "stb_image.h"
#include "FileLoader.h"
#include "content/ModelLoader.h"

namespace Audace
{
	std::string FileLoader::textFileToString(std::string path)
	{
		std::ifstream fin(basePath + path, std::ios::in);
		std::stringstream ss;
		ss << fin.rdbuf();
		return ss.str();
	}

	ImageData FileLoader::readImageFile(std::string path)
	{
		int width;
		int height;
		unsigned char *bytes = stbi_load((basePath + path).c_str(), &width, &height, nullptr, 0);
		ImageData img(bytes, width, height, GL_RGB);
		return img;
	}

	Model *FileLoader::readModelFile(std::string path, std::string filename)
	{
		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(basePath + path + filename, 0);
		if (scene == nullptr)
		{
			AU_ENGINE_LOG_ERROR("Assimp load error: {}", importer.GetErrorString());
			return nullptr;
		}

		int baseVertex = 0;
		Model *model = new Model;
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh *mesh = scene->mMeshes[i];

			ModelSection *section = new ModelSection;
			model->sections.push_back(section);

			aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];
			section->material = new Audace::Material;

			aiString name;
			mat->Get(AI_MATKEY_NAME, name);
			section->material->setName(name.C_Str());
			aiColor3D color;
			mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
			section->material->setAmbientColor(glm::vec3(color.r, color.g, color.b));
			mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
			section->material->setDiffuseColor(glm::vec3(color.r, color.g, color.b));
			mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
			section->material->setSpecularColor(glm::vec3(color.r, color.g, color.b));

			for (int j = 0; j < mesh->mNumVertices; j++)
			{
				aiVector3D pos = mesh->mVertices[j];
				aiVector3D normal = mesh->mNormals[j];
				// aiVector3D texCoord = mesh->mTextureCoords[0][j];

				Audace::Vertex vert;
				vert.position = glm::vec3(pos.x, pos.y, pos.z);
				vert.normal = glm::vec3(normal.x, normal.y, normal.z);
				// vert.texCoord = glm::vec3(texCoord.x, texCoord.y, texCoord.z);

				model->vertices.push_back(vert);
			}

			for (int j = 0; j < mesh->mNumFaces; j++)
			{
				aiFace face = mesh->mFaces[j];
				for (int k = 0; k < face.mNumIndices; k++)
				{
					section->indices.push_back(face.mIndices[k] + baseVertex);
				}
			}
			baseVertex += mesh->mNumVertices;
		}
		return model;
	}
}