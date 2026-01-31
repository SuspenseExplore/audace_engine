//
// Created by Suspense on 5/13/2023.
//

#include "AuLogger.h"
#include "AppController.h"
#include "content/AssetStore.h"
#include "scene/ForwardCamera.h"
#include "scene/NavigationScene.h"
#include "scene/SceneBuilder.h"
#include "scene/GltfViewerScene.h"
#include "scene/BinocularViewScene.h"

namespace Audace
{
    bool AppController::createWindow()
    {
        fileLoader = new FileAccessAndroid(androidApp);

        AssetStore::init(fileLoader);
        window.open(androidApp);
        return true;
    }

    void AppController::windowInitialized()
    {
        scene = new NavigationScene(this);
        scene->loadAssets(fileLoader);
    }

    void AppController::pollSystemEvents(android_app *app)
    {
        int id;
        int events;
        struct android_poll_source *source;
        while ((id = ALooper_pollOnce(0, nullptr, &events, (void **)&source)) >= 0)
        {
            if (source != nullptr)
            {
                source->process(app, source);
            }
        }
    }

    void AppController::runGameLoop()
    {
        while (true)
        {
            pollSystemEvents(androidApp);

            if (androidApp->destroyRequested != 0)
            {
                shutdown();
                window.close();
                AU_ENGINE_LOG_TRACE("Exiting render loop");
                AU_ENGINE_LOG_TRACE("Application terminating normally");
                return;
            }
            if (nextScene != CURRENT)
            {
                startNextScene();
            }

            renderFrame();
        }
    }

    void AppController::setScene(int scene)
    {
        nextScene = scene;
    }

    void AppController::startNextScene()
    {
        scene->disposeAssets();
        delete scene;

        switch (nextScene)
        {
        case NAVIGATION:
            scene = new NavigationScene(this);
            scene->loadAssets(fileLoader);
            break;

        case MAIN:
        {
            scene = new GltfViewerScene(this);
            BaseCamera *camera = Audace::ForwardCamera::standard3d(glm::vec3(0, -10, 2),
                                                                   getWidth(), getHeight());
            scene->setCamera(camera);
            scene->loadAssets(fileLoader);
        }
        break;

        case BUILDER:
            scene = new SceneBuilder(this);
            scene->loadAssets(fileLoader);
            break;
        }
        nextScene = CURRENT;
    }

    void AppController::renderFrame()
    {
        window.beginFrame();
        scene->render();
        scene->renderUi();
        window.endFrame();
    }

    void AppController::shutdown()
    {
        scene->disposeAssets();
        delete scene;
    }
}