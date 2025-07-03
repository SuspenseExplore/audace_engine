//
// Created by Suspense on 5/12/2023.
//

#ifndef AU_FILEACCESSANDROID_H
#define AU_FILEACCESSANDROID_H

#include "content/IFileAccess.h"
#include <jni.h>
#include <android_native_app_glue.h>
#include <string>
#include <map>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace Audace
{
	class ByteBuffer;
	class ImageData;
	class Model;

	class FileAccessAndroid : public IFileAccess
	{
		std::string externalFilePath;

	public:
		FileAccessAndroid() {}
		ByteBuffer* readFileToBuffer(const std::string& path) override;
		std::string textFileToString(const std::string& path) override;
		json textFileToJson(const std::string& path) override;
		ImageData readImageFile(const std::string& path) override;
		Model* readModelFile(std::string path, std::string filename) override;

		std::string assetReadBasePath() override { return ""; }
		std::string fileWriteBasePath() override { return externalFilePath; }

		void setExternalFilePath(std::string path) override { externalFilePath = path + "assets/"; }
	};
}

#endif
