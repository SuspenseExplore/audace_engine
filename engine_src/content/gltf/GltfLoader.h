#ifndef AU_GLTFLOADER_H
#define AU_GLTFLOADER_H

#include <string>
#include <vector>
#include <map>
#include "au_renderer.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

using namespace std;

namespace Audace
{
	class ByteBuffer;
	class IFileAccess;
	class ImageData;
	class DataBuffer;
	class Sprite;
	class BaseMaterial;
	class Texture2d;
	class PointLight;
	class DirLight;
	class Scene;
	class SceneGraph;
	class SceneGraphNode;
	class INodeAnimation;

	struct GltfBuffer
	{
		int id;
		ByteBuffer* byteBuf;
		int byteLength;

		DataBuffer* glBuffer;
	};
	struct GltfBufferView
	{
		int id;
		int bufferId;
		int byteOffset = 0;
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
		int materialId = -1;
	};
	struct GltfImage
	{
		ImageData* imgData;
	};
	struct GltfTexSampler
	{
		int minFilter;
		int magFilter;
		int wrapS;
		int wrapT;
	};
	struct GltfMesh
	{
		vector<GltfPrimitive> primitives;
	};
	struct GltfNode
	{
		string name;
		vector<int> childNodeIds;
		int meshId = -1;
		glm::vec3 translation{ 0, 0, 0 };
		glm::vec3 scale{ 1, 1, 1 };
		glm::quat rotation{ 1, 0, 0, 0 };
		glm::mat4 localTransform = glm::mat4(1.0);
		vector<int> animationIds;
		json* extensions = nullptr;
	};
	struct GltfAnimSampler
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
		vector<GltfAnimSampler> samplers;
		vector<GltfChannel> channels;
	};
	struct GltfScene
	{
		vector<int> nodeIds;
	};
	struct GltfFile
	{
		string filename;
		string filepath;
		string version;
	};

	class GltfLoader
	{
		IFileAccess* fileLoader;
		GltfFile fileData;
		string imageLoadPath;
		json jRoot;

		int defaultSceneId = 0;
		vector<GltfScene> scenes;
		vector<GltfNode> nodes;
		vector<GltfAnimation> animations;
		vector<GltfImage> images;
		vector<GltfTexSampler> texSamplers;
		vector<Texture2d*> textures;
		map<int, Sprite*> lights;
		map<int, string> lightTypes;
		vector<GltfMesh> meshes;
		vector<GltfBuffer> buffers;
		vector<GltfBufferView> bufferViews;
		vector<GltfAccessor> accessors;

		vector<BaseMaterial*> materials;
		vector<Sprite*> sprites;

		void parseBuffers(json& jBuffers);
		void parseBufferViews(json& jBufViews);
		void parseAccessors(json& jAccessors);
		void parseMeshes(json& jMeshes);
		void parseNodes(json& jNodes);
		void parseAnimations(json& jAnimations);
		void parseScenes(json& jScenes);
		void parseImages(json& jImages);
		void parseTexSamplers(json& jTexSamplers);
		void parseTextures(json& jTextures);
		void parseMaterials(json& jMaterials);
		void parseLights(json& jLights);

		char* getDataChunk(int bufferViewId, int startByte);
		vector<unsigned short> getDataUShort(int accessorId);
		vector<float> getDataFloat(int accessorId);
		vector<glm::vec2> getDataVec2(int accessorId);
		vector<glm::vec3> getDataVec3(int accessorId);
		vector<glm::vec4> getDataVec4(int accessorId);
		vector<glm::quat> getDataQuat(int accessorId);
		SceneGraphNode* getNode(int id);
		vector<INodeAnimation*> getAnimations(int nodeId);

	public:
		void loadFile(IFileAccess* fileLoader, std::string path, std::string filename);
		void setImageLoadPath(string p);
		Sprite* getSprite(int meshId);
		SceneGraph* getSceneGraph(Scene* scene);
		SceneGraph* getSceneGraph(Scene* scene, SceneGraphNode* root);
	};
}

#endif