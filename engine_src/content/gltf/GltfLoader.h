#ifndef AU_GLTFLOADER_H
#define AU_GLTFLOADER_H

#include <string>
#include <vector>
#include <map>
#include "content/IFileAccess.h"
#include "content/ByteBuffer.h"
#include "renderer/Sprite.h"
#include "scene/graph/SceneGraph.h"
#include "scene/graph/SceneGraphNode.h"
#include "scene/graph/INodeAnimation.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

using namespace std;

namespace Audace
{
	struct GltfBuffer
	{
		int id;
		ByteBuffer* buffer;
		int byteLength;
	};
	struct GltfBufferView
	{
		int id;
		int bufferId;
		int byteOffset;
		int byteLength;
		int byteStride = 0;
		int target = GL_ARRAY_BUFFER;
	};
	struct GltfAccessor
	{
		int id;
		int bufferViewId;
		int byteOffset;
		int componentType;
		int count;
		string type;
		vector<float> max;
		vector<float> min;
	};
	struct GltfPrimitive
	{
		map<string, int> attrAccessorIds;
		int indAccessorId;
		bool isIndexed;
		int mode = GL_TRIANGLES;
	};
	struct GltfMesh
	{
		vector<GltfPrimitive> primitives;
	};
	struct GltfNode
	{
		vector<int> childNodeIds;
		int meshId = -1;
		glm::vec3 translation{ 0, 0, 0 };
		glm::vec3 scale{ 1, 1, 1 };
		glm::quat rotation{ 1, 0, 0, 0 };
		glm::mat4 localTransform = glm::mat4(1.0);
		vector<int> animationIds;
	};
	struct GltfSampler
	{
		int inputAccessorId;
		int outputAccessorId;
		string interpolation;
	};
	struct GltfChannel
	{
		int samplerId;
		int targetNodeId;
		string targetPath;
	};
	struct GltfAnimation
	{
		int id;
		vector<GltfSampler> samplers;
		vector<GltfChannel> channels;
	};
	struct GltfScene
	{
		vector<int> nodeIds;
	};
	struct GltfFile
	{
		string filename;
		string version;
	};

	class GltfLoader
	{
		GltfFile fileData;
		int defaultSceneId = 0;
		vector<GltfScene> scenes;
		vector<GltfNode> nodes;
		vector<GltfAnimation> animations;
		vector<GltfMesh> meshes;
		vector<GltfBuffer> buffers;
		vector<GltfBufferView> bufferViews;
		vector<GltfAccessor> accessors;

		vector<Sprite*> sprites;

		void parseBuffers(json& jBuffers);
		void parseBufferViews(json& jBufViews);
		void parseAccessors(json& jAccessors);
		void parseMeshes(json& jMeshes);
		void parseNodes(json& jNodes);
		void parseAnimations(json& jAnimations);
		void parseScenes(json& jScenes);

		char* getDataChunk(int bufferViewId, int startByte);
		vector<unsigned short> getDataUShort(int accessorId);
		vector<float> getDataFloat(int accessorId);
		vector<glm::vec3> getDataVec3(int accessorId);
		vector<glm::vec4> getDataVec4(int accessorId);
		vector<glm::quat> getDataQuat(int accessorId);
		Sprite* getSprite(int meshId);
		SceneGraphNode* getNode(int id);
		vector<INodeAnimation*> getAnimations(int nodeId);

	public:
		void loadFile(IFileAccess* fileLoader, std::string path, std::string filename);
		SceneGraph* getSceneGraph(Scene* scene);
	};
}

#endif