#ifndef AU_GLTFXREADER_H
#define AU_GLTFXREADER_H

#include <string>
#include <vector>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace Audace
{
	class IFileAccess;
	class Scene;
	class SceneGraph;
	class SceneGraphNode;

	class GltfxReader
	{
		IFileAccess* fileLoader;
		json jroot;
		std::vector<SceneGraphNode*> assets;

	public:
		GltfxReader(IFileAccess* fileLoader);

		SceneGraphNode* readAsset(int assetId);
		std::vector<SceneGraphNode*> getAssets();
		SceneGraphNode* readNode(int nodeId);
		SceneGraphNode* readDefaultScene(const std::string& filepath);
	};
}

#endif