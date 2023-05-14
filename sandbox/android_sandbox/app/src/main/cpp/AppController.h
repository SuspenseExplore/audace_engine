//
// Created by Suspense on 5/13/2023.
//

#ifndef AU_APPCONTROLLER_H
#define AU_APPCONTROLLER_H

#include <jni.h>
#include <android_native_app_glue.h>
#include "EngineEventListener.h"
#include "EglWindow.h"
#include "FileLoader.h"
#include "renderer/DataBuffer.h"
#include "renderer/ShaderProgram.h"
#include "renderer/VertexAttribute.h"
#include "renderer/VertexArray.h"

namespace Audace {
	class AppController : public EngineEventListener {
		android_app *androidApp;
		FileLoader *fileLoader;
		DataBuffer *buffer;
		ShaderProgram *shader;
		VertexArray *vertexArray;

	public:
		EglWindow window;

		AppController(android_app *app) : androidApp(app) {
			fileLoader = new FileLoader(app->activity->assetManager);
		}
		bool createWindow();
		void runGameLoop();
		void shutdown();

		static void pollSystemEvents(android_app *app);

		void windowInitialized() override;
	};
}

#endif //AU_APPCONTROLLER_H
