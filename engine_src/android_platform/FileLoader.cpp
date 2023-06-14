//
// Created by Suspense on 5/12/2023.
//

#include "FileLoader.h"
#include "stb_image.h"
#include "au_renderer.h"
#include "content/ModelLoader.h"

namespace Audace {

	std::string FileLoader::textFileToString(const std::string& path) {
		AAsset *asset = getAsset(path);
		long length = AAsset_getLength(asset);
		char *buf = reinterpret_cast<char*>(readAssetToBuffer(asset));
		std::string s(buf, length);
		delete[] buf;
		return s;
	}

	ImageData FileLoader::readImageFile(const std::string& path) {
		int bytesRead = 0;
		AAsset *asset = AAssetManager_open(assetManager, path.c_str(), AASSET_MODE_STREAMING);
		long size = AAsset_getLength(asset);
		char *buf = readAssetToBuffer(asset);

		int width, height, channels;
		stbi_uc* bytes = stbi_load_from_memory(reinterpret_cast<stbi_uc*>(buf), size, &width, &height, &channels, 3);
		delete[] buf;
		ImageData img(bytes, width, height, GL_RGB);
		return img;
	}

	char* FileLoader::readAssetToBuffer(AAsset *asset) {
		int bytesRead = 0;
		long size = AAsset_getLength(asset);
		char *buf = new char[size];
		bytesRead = AAsset_read(asset, buf, size);
		AAsset_close(asset);

		return buf;
	}

	Model *FileLoader::readModelFile(std::string path, std::string filename)
	{
		Model* model = ModelLoader::loadObj(this, path, filename);
		return model;
	}

	AAsset* FileLoader::getAsset(const std::string& path) {
		return AAssetManager_open(assetManager, path.c_str(), AASSET_MODE_BUFFER);
	}
}