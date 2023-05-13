//
// Created by Suspense on 5/12/2023.
//

#include "FileLoader.h"

namespace Audace {

	std::string FileLoader::textFileToString(std::string path) {
		int bytesRead = 0;
		AAsset *asset = AAssetManager_open(assetManager, path.c_str(), AASSET_MODE_BUFFER);
		long size = AAsset_getLength(asset);
		char *buf = new char[size];
		bytesRead = AAsset_read(asset, buf, size);
		std::string s(buf, size);
		AAsset_close(asset);
		delete[] buf;
		return s;
	}
}