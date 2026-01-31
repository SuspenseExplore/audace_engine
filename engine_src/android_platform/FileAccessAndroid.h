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
        FileAccessAndroid(android_app *app)
        {
            // find the proper data folder for assets
            // from: https://stackoverflow.com/questions/10683119/android-app-activity-internaldatapath-still-null-in-2-3-8-ndk-r8
            const char *path = app->activity->internalDataPath;
            if (!path)
            {
                JNIEnv *jni;
                app->activity->vm->AttachCurrentThread(&jni, nullptr);
                jclass activityClass = jni->GetObjectClass(app->activity->clazz);
                jmethodID getFilesDir = jni->GetMethodID(activityClass, "getFilesDir",
                                                         "()Ljava/io/File;");
                jobject fileObject = jni->CallObjectMethod(app->activity->clazz, getFilesDir);
                jclass fileClass = jni->GetObjectClass(fileObject);
                jmethodID getAbsolutePath = jni->GetMethodID(fileClass, "getAbsolutePath",
                                                             "()Ljava/lang/String;");
                jobject pathObject = jni->CallObjectMethod(fileObject, getAbsolutePath);
                path = jni->GetStringUTFChars((jstring)pathObject, nullptr);

                jni->DeleteLocalRef(pathObject);
                jni->DeleteLocalRef(fileClass);
                jni->DeleteLocalRef(fileObject);
                jni->DeleteLocalRef(activityClass);

                app->activity->vm->DetachCurrentThread();
            }

            externalFilePath = path;
            externalFilePath += "/assets/";
        }
        ByteBuffer *readFileToBuffer(const std::string &path) override;
        std::string textFileToString(const std::string &path) override;
        json textFileToJson(const std::string &path) override;
        ImageData readImageFile(const std::string &path) override;
        ImageData readHdrImageFile(const std::string &path) override;
        Model *readModelFile(std::string path, std::string filename) override;

        std::string assetReadBasePath() override { return ""; }
        std::string fileWriteBasePath() override { return externalFilePath; }

        void setExternalFilePath(std::string path) override { externalFilePath = path + "/assets/"; }
    };
}

#endif
