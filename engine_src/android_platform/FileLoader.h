//
// Created by Suspense on 5/12/2023.
//

#ifndef AU_FILELOADER_H
#define AU_FILELOADER_H

#include <jni.h>
#include <android_native_app_glue.h>
#include <string>
#include <map>
#include "ImageData.h"
#include "content/Model.h"
#include "renderer/ShaderProgram.h"

namespace Audace {
	class FileLoader {
		AAssetManager *assetManager;

		AAsset* getAsset(const std::string& path);
		char* readAssetToBuffer(AAsset *asset);

	public:
		FileLoader(AAssetManager *assets) : assetManager(assets) {}
		std::string textFileToString(const std::string& path);
		ImageData readImageFile(const std::string& path);
		Model* readModelFile(std::string path, std::string filename);
	};
}

#endif
