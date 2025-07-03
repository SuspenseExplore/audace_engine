//
// Created by Suspense on 5/12/2023.
//

#include <fstream>
#include "FileAccessAndroid.h"
#include "stb_image.h"
#include "au_renderer.h"
#include "content/ByteBuffer.h"
#include "content/ImageData.h"
#include "content/ModelLoader.h"

namespace Audace {

	std::string FileAccessAndroid::textFileToString(const std::string& path) {
		ByteBuffer* buf = readFileToBuffer(path);
		std::string r = std::string(buf->getBuffer());
		return r.substr(0, buf->getLength());
	}

	json FileAccessAndroid::textFileToJson(const std::string& path) {
		return json::parse(textFileToString(path));
	}

	ByteBuffer* FileAccessAndroid::readFileToBuffer(const std::string& path) {
		std::ifstream fin(fileWriteBasePath() + path, std::ios::in | std::ios::binary | std::ios::ate);
		int size = fin.tellg();
		fin.seekg(0);
		char* buf = new char[size];
		fin.read(buf, size);
		return new ByteBuffer(buf, size);
	}

	ImageData FileAccessAndroid::readImageFile(const std::string& path) {
		ByteBuffer* buf = readFileToBuffer(path);

		int width, height, channels;
		stbi_uc* bytes = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(buf->getBuffer()), buf->getLength(), &width, &height, &channels, 3);
		delete[] buf;
		ImageData img(bytes, width, height, GL_RGB);
		return img;
	}

	Model* FileAccessAndroid::readModelFile(std::string path, std::string filename)
	{
		// filename could have part of the path, so put them together and them take them apart again
		std::string fullName = path + filename;
		int i = fullName.find_last_of('/') + 1;
		Model* model = ModelLoader::loadObj(this, fullName.substr(0, i), fullName.substr(i));
		return model;
	}
}