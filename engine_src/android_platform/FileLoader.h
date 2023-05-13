//
// Created by Suspense on 5/12/2023.
//

#ifndef AU_FILELOADER_H
#define AU_FILELOADER_H

#include <jni.h>
#include <android_native_app_glue.h>
#include <string>
#include <map>
#include "renderer/ShaderProgram.h"

namespace Audace {
	class FileLoader {
		AAssetManager *assetManager;

	public:
		FileLoader(AAssetManager *assets) : assetManager(assets) {}
		std::string textFileToString(std::string path);

	};
}

#endif
