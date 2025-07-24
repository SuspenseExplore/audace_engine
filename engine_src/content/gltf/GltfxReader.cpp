#include "GltfxReader.h"
#include "content/IFileAccess.h"
#include "content/JsonSerializer.h"
#include "content/gltf/GltfLoader.h"
#include "renderer/Sprite.h"
#include "scene/Scene.h"
#include "scene/graph/SceneGraph.h"
#include "scene/graph/SceneGraphNode.h"

namespace Audace
{
	typedef JsonSerializer jser;

	GltfxReader::GltfxReader(IFileAccess* fileLoader) : fileLoader(fileLoader)
	{

	}

	SceneGraphNode* GltfxReader::readAsset(int assetId)
	{
		std::string filename = jroot["assets"][assetId]["uri"].template get<std::string>();

		if (filename.find(".gltfx") == -1)
		{
			GltfLoader loader;
			loader.setImageLoadPath("images/quaternius/");
			int i = filename.find_last_of("/") + 1;
			loader.loadFile(fileLoader, filename.substr(0, i), filename.substr(i));
			return loader.getSceneRootNode(0);
		}

		GltfxReader reader(fileLoader);
		return reader.readDefaultScene(filename);
	}

	SceneGraphNode* GltfxReader::readNode(int nodeId)
	{
		json& jnode = jroot["nodes"][nodeId];
		SceneGraphNode* node = new SceneGraphNode();
		if (jnode.contains("asset"))
		{
			int assetId = jnode["asset"].template get<int>();
			delete node;
			node = assets[assetId];
		}
		if (jnode.contains("name"))
		{
			node->setName(jnode["name"].template get<std::string>());
		}
		if (jnode.contains("translation"))
		{
			node->setTranslation(jser::getVec3(jnode, "translation"));
		}
		if (jnode.contains("scale"))
		{
			node->setScale(jser::getVec3(jnode, "scale"));
		}
		if (jnode.contains("rotation"))
		{
			node->setRotation(jser::getQuat(jnode, "rotation"));
		}
		for (int i = 0; i < jnode["children"].size(); i++)
		{
			int child = jnode["children"][i].template get<int>();
			node->addChild(readNode(child));
		}
		return node;
	}

	std::vector<SceneGraphNode*> GltfxReader::getAssets()
	{
		return assets;
	}

	/**
	 * Return the scene graph tree for the file's default scene.
	 */
	SceneGraphNode* GltfxReader::readDefaultScene(const std::string& filepath)
	{
		jroot = fileLoader->textFileToJson(filepath);
		json& jassets = jroot["assets"];

		assets.resize(jassets.size());
		for (int i = 0; i < jassets.size(); i++)
		{
			assets[i] = readAsset(i);
		}
		int sceneId = jser::getInt(jroot, "scene");
		json& jscene = jroot["scenes"][sceneId];
		int nodeId = jscene["nodes"][0].template get<int>();
		SceneGraphNode* node = readNode(nodeId);

		return node;
	}
}