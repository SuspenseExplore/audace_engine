#include "GltfLoader.h"
#include "content/JsonSerializer.h"
#include "b64/decode.h"
#include <sstream>
#include "content/IFileAccess.h"
#include "content/AssetStore.h"
#include "content/ByteBuffer.h"
#include "content/ImageData.h"
#include "scene/Scene.h"
#include "scene/graph/SceneGraph.h"
#include "scene/graph/SceneGraphNode.h"
#include "scene/graph/RotationAnimation.h"
#include "renderer/DataBuffer.h"
#include "renderer/VertexAttribute.h"
#include "renderer/VertexArray.h"
#include "renderer/Mesh.h"
#include "renderer/Texture2d.h"
#include "renderer/Sprite.h"
#include "renderer/material/PbrMetalRoughMat.h"
#include "renderer/light/PointLight.h"
#include "renderer/light/DirLight.h"
#include "renderer/light/SpotLight.h"

namespace Audace
{
	typedef JsonSerializer jser;

	void GltfLoader::parseBuffers(json &jBuffers)
	{
		buffers.resize(jBuffers.size());
		for (int i = 0; i < jBuffers.size(); i++)
		{
			json &jBuffer = jBuffers[i];
			GltfBuffer &gltfBuffer = buffers[i];
			gltfBuffer.id = i;
			gltfBuffer.byteLength = jBuffer["byteLength"];
			if (!jBuffer.contains("uri"))
			{
				gltfBuffer.byteBuf = dataChunk;
			}
			else
			{
				string uri = jser::getString(jBuffer, "uri");
				int ind = uri.find(";base64,");
				if (ind > -1)
				{
					uri = uri.substr(ind + 8);
					base64::decoder decoder(gltfBuffer.byteLength);
					stringstream sin = stringstream(uri);
					stringstream sout;
					decoder.decode(sin, sout);
					char *bytes = new char[gltfBuffer.byteLength];
					memset(bytes, 0, gltfBuffer.byteLength);
					memcpy(bytes, sout.str().c_str(), gltfBuffer.byteLength);
					ByteBuffer *byteBuf = new ByteBuffer(bytes, gltfBuffer.byteLength);
					gltfBuffer.byteBuf = byteBuf;
				}
				else
				{
					ind = uri.find(".bin");
					if (ind > -1)
					{
						ByteBuffer *b = fileLoader->readFileToBuffer(fileData.filepath + uri);
						gltfBuffer.byteBuf = b;
					}
				}
			}
			gltfBuffer.glBuffer = new DataBuffer(gltfBuffer.byteBuf->getBuffer(), gltfBuffer.byteLength, GL_ARRAY_BUFFER, GL_STATIC_DRAW);
			gltfBuffer.glBuffer->create();
		}
	}

	void GltfLoader::parseBufferViews(json &jBufViews)
	{
		bufferViews.resize(jBufViews.size());
		for (int i = 0; i < jBufViews.size(); i++)
		{
			json &jView = jBufViews[i];
			GltfBufferView &gltfView = bufferViews[i];
			gltfView.id = i;
			gltfView.bufferId = jser::getInt(jView, "buffer");
			if (jView.contains("byteOffset"))
			{
				gltfView.byteOffset = jser::getInt(jView, "byteOffset");
			}
			gltfView.byteLength = jser::getInt(jView, "byteLength");
			if (jView.contains("byteStride"))
			{
				gltfView.byteStride = jser::getInt(jView, "byteStride");
			}
			if (jView.contains("target"))
			{
				gltfView.target = jser::getInt(jView, "target");
			}
		}
	}

	void GltfLoader::parseAccessors(json &jAccessors)
	{
		accessors.resize(jAccessors.size());
		for (int i = 0; i < jAccessors.size(); i++)
		{
			json &jAccessor = jAccessors[i];
			GltfAccessor &gltfAccessor = accessors[i];
			gltfAccessor.id = i;
			gltfAccessor.bufferViewId = jser::getInt(jAccessor, "bufferView");
			if (jAccessor.contains("byteOffset"))
			{
				gltfAccessor.byteOffset = jser::getInt(jAccessor, "byteOffset");
			}
			gltfAccessor.componentType = jser::getInt(jAccessor, "componentType");
			gltfAccessor.count = jser::getInt(jAccessor, "count");
			gltfAccessor.type = jser::getString(jAccessor, "type");
			if (gltfAccessor.type == "VEC4")
			{
				gltfAccessor.min.resize(4);
				gltfAccessor.max.resize(4);
			}
			else if (gltfAccessor.type == "VEC3")
			{
				gltfAccessor.min.resize(3);
				gltfAccessor.max.resize(3);
			}
			else if (gltfAccessor.type == "VEC2")
			{
				gltfAccessor.min.resize(2);
				gltfAccessor.max.resize(2);
			}
			else if (gltfAccessor.type == "SCALAR")
			{
				gltfAccessor.min.resize(1);
				gltfAccessor.max.resize(1);
			}
			for (int j = 0; j < jAccessor["min"].size(); j++)
			{
				gltfAccessor.min[j] = jAccessor["min"][j];
				gltfAccessor.max[j] = jAccessor["max"][j];
			}

			if (jAccessor.contains("sparse"))
			{
				json &node = jAccessor["sparse"];
				json &indexNode = node["indices"];
				int count = jser::getInt(node, "count");
				GltfBufferView bvIndex = bufferViews[jser::getInt(indexNode, "bufferView")];
				int indexOffset = jser::getInt(indexNode, "byteOffset");
				int indexType = jser::getInt(indexNode, "componentType");
				unsigned short *indexData = (unsigned short *)getDataChunk(bvIndex.bufferId, indexOffset);

				json &valueNode = node["values"];
				int bvValuesId = jser::getInt(valueNode, "bufferView");
				int valueOffset = jser::getInt(valueNode, "byteOffset");
				float *valueData = (float *)getDataChunk(bvValuesId, valueOffset);
				float *originalData = (float *)getDataChunk(gltfAccessor.bufferViewId, gltfAccessor.byteOffset);
				for (int j = 0; j < count; j++)
				{
					unsigned short ind = indexData[j];
					memcpy(originalData + ind * 3, valueData + j * 3, 12);
				}
			}
		}
	}

	void GltfLoader::parseMeshes(json &jMeshes)
	{
		meshes.resize(jMeshes.size());
		for (int i = 0; i < jMeshes.size(); i++)
		{
			json &jMesh = jMeshes[i];
			json &jPrimitives = jMesh["primitives"];
			vector<GltfPrimitive> &gltfPrimitives = meshes[i].primitives;
			gltfPrimitives.resize(jPrimitives.size());
			for (int j = 0; j < jPrimitives.size(); j++)
			{
				json &jPrim = jPrimitives[j];
				GltfPrimitive &gltfPrim = gltfPrimitives[j];
				json &jAttrs = jPrim["attributes"];
				for (auto &jAttr : jAttrs.items())
				{
					int ind = jAttr.value().template get<int>();
					gltfPrim.attrAccessorIds[jAttr.key()] = ind;
					if (jAttr.key() == "POSITION")
					{
						meshes[i].bbox.min.x = min(accessors[ind].min[0], meshes[i].bbox.min.x);
						meshes[i].bbox.min.y = min(accessors[ind].min[1], meshes[i].bbox.min.y);
						meshes[i].bbox.min.z = min(accessors[ind].min[2], meshes[i].bbox.min.z);
						meshes[i].bbox.max.x = max(accessors[ind].max[0], meshes[i].bbox.max.x);
						meshes[i].bbox.max.y = max(accessors[ind].max[1], meshes[i].bbox.max.y);
						meshes[i].bbox.max.z = max(accessors[ind].max[2], meshes[i].bbox.max.z);
					}
				}
				if (jPrim.contains("mode"))
				{
					gltfPrim.mode = jser::getInt(jPrim, "mode");
				}
				if (jPrim.contains("indices"))
				{
					gltfPrim.isIndexed = true;
					int ind = jser::getInt(jPrim, "indices");
					gltfPrim.indAccessorId = ind;
				}
				else
				{
					gltfPrim.isIndexed = false;
				}
				if (jPrim.contains("material"))
				{
					gltfPrim.materialId = jser::getInt(jPrim, "material");
				}
			}
		}
	}

	void GltfLoader::parseNodes(json &jNodes)
	{
		nodes.resize(jNodes.size());
		for (int i = 0; i < jNodes.size(); i++)
		{
			json &jNode = jNodes[i];
			GltfNode &node = nodes[i];
			if (jNode.contains("name"))
			{
				node.name = jser::getString(jNode, "name");
			}
			if (jNode.contains("children"))
			{
				for (int j = 0; j < jNode["children"].size(); j++)
				{
					node.childNodeIds.emplace_back(jNode["children"][j]);
				}
			}
			if (jNode.contains("mesh"))
			{
				node.meshId = jser::getInt(jNode, "mesh");
			}
			if (jNode.contains("translation"))
			{
				node.translation = jser::getVec3(jNode, "translation");
			}
			if (jNode.contains("scale"))
			{
				node.scale = jser::getVec3(jNode, "scale");
			}
			if (jNode.contains("rotation"))
			{
				node.rotation = jser::getQuat(jNode, "rotation");
			}
			if (jNode.contains("extensions"))
			{
				node.extensions = &jNode["extensions"];
			}
		}
	}

	void GltfLoader::parseAnimations(json &jAnimations)
	{
		animations.resize(jAnimations.size());
		for (int i = 0; i < jAnimations.size(); i++)
		{
			json &jAnimation = jAnimations[i];
			GltfAnimation &animation = animations[i];
			animation.id = i;

			json &jSamplers = jAnimation["samplers"];
			animation.samplers.resize(jSamplers.size());
			for (int j = 0; j < jSamplers.size(); j++)
			{
				json &jSampler = jSamplers[j];
				GltfAnimSampler &sampler = animation.samplers[j];
				sampler.inputAccessorId = jser::getInt(jSampler, "input");
				sampler.outputAccessorId = jser::getInt(jSampler, "output");
				sampler.interpolation = jser::getString(jSampler, "interpolation");
			}

			json &jChannels = jAnimation["channels"];
			animation.channels.resize(jChannels.size());
			for (int j = 0; j < jChannels.size(); j++)
			{
				json &jChannel = jChannels[j];
				GltfChannel &channel = animation.channels[j];
				channel.samplerId = jser::getInt(jChannel, "sampler");
				channel.targetNodeId = jser::getInt(jChannel["target"], "node");
				channel.targetPath = jser::getString(jChannel["target"], "path");
				nodes[channel.targetNodeId].animationIds.emplace_back(animation.id);
			}
		}
	}

	void GltfLoader::parseImages(json &jImages)
	{
		images.resize(jImages.size());
		for (int i = 0; i < jImages.size(); i++)
		{
			json &jImg = jImages[i];

			string uri = jser::getString(jImg, "uri");
			AU_ENGINE_LOG_DEBUG("Loading file: {}", uri);
			ImageData t = fileLoader->readImageFile(imageLoadPath + uri);
			AU_ENGINE_LOG_DEBUG("Done loading file: {}", uri);
			ImageData *data = new ImageData(t.bytes, t.width, t.height, t.format);
			images[i].imgData = data;
		}
	}

	void GltfLoader::parseTexSamplers(json &jTexSamplers)
	{
		texSamplers.resize(jTexSamplers.size());
		for (int i = 0; i < jTexSamplers.size(); i++)
		{
			json &jSampler = jTexSamplers[i];
			if (jSampler.contains("minFilter"))
			{
				texSamplers[i].minFilter = jser::getInt(jSampler, "minFilter");
			}
			if (jSampler.contains("magFilter"))
			{
				texSamplers[i].magFilter = jser::getInt(jSampler, "magFilter");
			}
			if (jSampler.contains("wrapS"))
			{
				texSamplers[i].wrapS = jser::getInt(jSampler, "wrapS");
			}
			if (jSampler.contains("wrapT"))
			{
				texSamplers[i].wrapT = jser::getInt(jSampler, "wrapT");
			}
		}
	}

	void GltfLoader::parseTextures(json &jTextures)
	{
		textures.reserve(jTextures.size());
		for (int i = 0; i < jTextures.size(); i++)
		{
			json &jTex = jTextures[i];
			int source = jser::getInt(jTex, "source");
			if (jTex.contains("sampler"))
			{
				int sampler = jser::getInt(jTex, "sampler");
			}
			Texture2d *tex = new Texture2d(*(images[source].imgData));
			tex->create();
			textures.emplace_back(tex);
		}
	}

	void GltfLoader::parseMaterials(json &jMaterials)
	{
		materials.reserve(jMaterials.size());
		for (json &jMat : jMaterials)
		{
			BaseMaterial *mat = nullptr;
			if (jMat.contains("pbrMetallicRoughness"))
			{
				json &jpbr = jMat["pbrMetallicRoughness"];
				PbrMetalRoughMat *pbrMat = new PbrMetalRoughMat();
				pbrMat->setShader(AssetStore::getShader("standard"));
				mat = pbrMat;
				if (jpbr.contains("baseColorFactor"))
				{
					pbrMat->setBaseColorFactor(jser::getVec4(jpbr, "baseColorFactor"));
				}
				if (jpbr.contains("metallicFactor"))
				{
					pbrMat->setMetallicFactor(jser::getFloat(jpbr, "metallicFactor"));
				}
				if (jpbr.contains("roughnessFactor"))
				{
					pbrMat->setRoughnessFactor(jser::getFloat(jpbr, "roughnessFactor"));
				}
				if (jpbr.contains("baseColorTexture"))
				{
					int ind = jser::getInt(jpbr["baseColorTexture"], "index");
					pbrMat->setBaseColorMap(textures[ind]);
				}
				if (jpbr.contains("metallicRoughnessTexture"))
				{
					int ind = jser::getInt(jpbr["metallicRoughnessTexture"], "index");
					pbrMat->setMetallicMap(textures[ind]);
					pbrMat->setRoughnessMap(textures[ind]);
				}

				// these aren't actually part of the PBR GLTF object.  they're here temporarily
				if (jMat.contains("emissiveFactor"))
				{
					pbrMat->setEmissiveFactor(jser::getVec3(jMat, "emissiveFactor"));
				}
				if (jMat.contains("normalTexture"))
				{
					int ind = jser::getInt(jMat["normalTexture"], "index");
					pbrMat->setNormalMap(textures[ind]);
				}
				if (jMat.contains("occlusionTexture"))
				{
					int ind = jser::getInt(jMat["occlusionTexture"], "index");
					pbrMat->setOcclusionMap(textures[ind]);
				}
				if (jMat.contains("emissiveTexture"))
				{
					int ind = jser::getInt(jMat["emissiveTexture"], "index");
					pbrMat->setEmissiveMap(textures[ind]);
				}
			}
			materials.emplace_back(mat);
		}
	}

	void GltfLoader::parseLights(json &jLights)
	{
		for (int i = 0; i < jLights.size(); i++)
		{
			json &jLight = jLights[i];
			if (jser::getString(jLight, "type") == "point")
			{
				PointLight *ptLight = new PointLight;
				ptLight->setColor(jser::getVec3(jLight, "color"));
				ptLight->setIntensity(jser::getFloat(jLight, "intensity"));
				if (jLight.contains("name"))
				{
					ptLight->setName(jser::getString(jLight, "name"));
				}
				lights[i] = ptLight;
				lightTypes[i] = "point";
			}
			else if (jser::getString(jLight, "type") == "directional")
			{
				DirLight *light = new DirLight;
				glm::vec3 color = jser::getVec3(jLight, "color");
				if (jLight.contains("intensity"))
				{
					light->setIntensity(jser::getFloat(jLight, "intensity"));
				}
				if (jLight.contains("name"))
				{
					light->setName(jser::getString(jLight, "name"));
				}
				light->setColor(color);
				lights[i] = light;
				lightTypes[i] = "directional";
			}
			else if (jser::getString(jLight, "type") == "spot")
			{
				SpotLight *light = new SpotLight;
				lights[i] = light;
				lightTypes[i] = "spot";
				light->setColor(jser::getVec3(jLight, "color"));
				if (jLight.contains("intensity"))
				{
					light->setIntensity(jser::getFloat(jLight, "intensity"));
				}
				if (jLight.contains("innerConeAngle"))
				{
					light->setInnerAngle(jser::getFloat(jLight, "innerConeAngle"));
				}
				if (jLight.contains("outerConeAngle"))
				{
					light->setOuterAngle(jser::getFloat(jLight, "outerConeAngle"));
				}
				if (jLight.contains("name"))
				{
					light->setName(jser::getString(jLight, "name"));
				}
			}
		}
	}

	void GltfLoader::parseScenes(json &jScenes)
	{
		scenes.resize(jScenes.size());
		for (int i = 0; i < jScenes.size(); i++)
		{
			json &jScene = jScenes[i];
			json &jNodes = jScene["nodes"];
			scenes[i].nodeIds.resize(jNodes.size());
			for (int j = 0; j < jNodes.size(); j++)
			{
				scenes[i].nodeIds[j] = jNodes[j];
			}
		}
	}

	void GltfLoader::setImageLoadPath(string p)
	{
		imageLoadPath = p;
	}

	void GltfLoader::loadFile(IFileAccess *fileLoader, std::string path, std::string filename)
	{
		this->fileLoader = fileLoader;
		fileData.filename = filename;
		fileData.filepath = path;
		int i = filename.find(".gltf");
		if (i > -1)
		{
			jRoot = fileLoader->textFileToJson(path + filename);
		}
		else if ((i = filename.find(".glb")) > -1)
		{
			ByteBuffer *b = fileLoader->readFileToBuffer(path + filename);
			char *data = b->getBuffer();
			unsigned int magic = *reinterpret_cast<unsigned int *>(data);
			unsigned int version = *reinterpret_cast<unsigned int *>(data + 4);
			unsigned int length = *reinterpret_cast<unsigned int *>(data + 8);
			if (magic != 0x46546C67)
			{
				AU_ENGINE_LOG_DEBUG("Error loading file {}: incorrect header ID", filename);
				return;
			}
			if (version != 2)
			{
				AU_ENGINE_LOG_DEBUG("Error loading file {}: incorrect version", filename);
				return;
			}

			data += 12;
			unsigned int jsonLength = *reinterpret_cast<unsigned int *>(data);
			jsonLength += (jsonLength % 4);
			unsigned int jsonType = *reinterpret_cast<unsigned int *>(data + 4);
			if (jsonType != 0x4e4f534a)
			{
				AU_ENGINE_LOG_DEBUG("Error loading file {}: incorrect json chunk type", filename);
				return;
			}

			std::string jstring = std::string(data + 8, jsonLength);
			jRoot = json::parse(jstring);

			data += 8 + jsonLength;
			unsigned int binLength = *reinterpret_cast<unsigned int *>(data);
			unsigned int binType = *reinterpret_cast<unsigned int *>(data + 4);
			if (binType != 0x004e4942)
			{
				AU_ENGINE_LOG_DEBUG("Error loading file {}: incorrect binary chunk type", filename);
				return;
			}
			binLength += (binLength % 4);
			dataChunk = new ByteBuffer(data + 8, binLength);
		}
		{
			json &jAsset = jRoot["asset"];
			fileData.version = jAsset["version"];
		}

		if (jRoot.contains("scene"))
		{
			defaultSceneId = jser::getInt(jRoot, "scene");
		}

		{
			json &jScenes = jRoot["scenes"];
			parseScenes(jScenes);
		}

		{
			json &jNodes = jRoot["nodes"];
			parseNodes(jNodes);
		}

		{
			json &jBuffers = jRoot["buffers"];
			parseBuffers(jBuffers);
		}

		{
			json &jBufViews = jRoot["bufferViews"];
			parseBufferViews(jBufViews);
		}

		{
			json &jAccessors = jRoot["accessors"];
			parseAccessors(jAccessors);
		}

		{
			json &jAnimations = jRoot["animations"];
			parseAnimations(jAnimations);
		}

		if (jRoot.contains("images"))
		{
			json &jImages = jRoot["images"];
			parseImages(jImages);
		}

		if (jRoot.contains("samplers"))
		{
			json &jSamplers = jRoot["samplers"];
			parseTexSamplers(jSamplers);
		}

		if (jRoot.contains("textures"))
		{
			json &jTextures = jRoot["textures"];
			parseTextures(jTextures);
		}

		if (jRoot.contains("materials"))
		{
			json &jMaterials = jRoot["materials"];
			parseMaterials(jMaterials);
		}

		if (jRoot.contains("extensions"))
		{
			json &jExt = jRoot["extensions"];
			if (jExt.contains("KHR_lights_punctual"))
			{
				json &jLights = jExt["KHR_lights_punctual"]["lights"];
				parseLights(jLights);
			}
		}

		{
			json &jMeshes = jRoot["meshes"];
			parseMeshes(jMeshes);
		}
	}

	char *GltfLoader::getDataChunk(int bufferViewId, int startByte = 0)
	{
		GltfBufferView &bv = bufferViews[bufferViewId];
		GltfBuffer &buffer = buffers[bv.bufferId];
		return buffer.byteBuf->getBuffer() + bv.byteOffset + startByte;
	}

	vector<unsigned short> GltfLoader::getDataUShort(int accessorId)
	{
		vector<unsigned short> ret;
		GltfAccessor &acc = accessors[accessorId];
		GltfBufferView &bv = bufferViews[acc.bufferViewId];
		unsigned short *data = (unsigned short *)getDataChunk(acc.bufferViewId, acc.byteOffset);
		for (int i = 0; i < acc.count; i++)
		{
			ret.emplace_back(data[i]);
		}

		return ret;
	}

	vector<float> GltfLoader::getDataFloat(int accessorId)
	{
		vector<float> ret;
		GltfAccessor &acc = accessors[accessorId];
		GltfBufferView &bv = bufferViews[acc.bufferViewId];
		float *data = (float *)getDataChunk(acc.bufferViewId, acc.byteOffset);
		for (int i = 0; i < acc.count; i++)
		{
			ret.emplace_back(data[i]);
		}

		return ret;
	}

	vector<glm::vec2> GltfLoader::getDataVec2(int accessorId)
	{
		vector<glm::vec2> ret;
		GltfAccessor &acc = accessors[accessorId];
		GltfBufferView &bv = bufferViews[acc.bufferViewId];
		float *data = (float *)getDataChunk(acc.bufferViewId, acc.byteOffset);
		for (int i = 0; i < acc.count; i++)
		{
			float x = data[i * 2 + 0];
			float y = data[i * 2 + 1];
			ret.emplace_back(glm::vec2(x, y));
		}

		return ret;
	}

	vector<glm::vec3> GltfLoader::getDataVec3(int accessorId)
	{
		vector<glm::vec3> ret;
		GltfAccessor &acc = accessors[accessorId];
		GltfBufferView &bv = bufferViews[acc.bufferViewId];
		float *data = (float *)getDataChunk(acc.bufferViewId, acc.byteOffset);
		for (int i = 0; i < acc.count; i++)
		{
			float x = data[i * 3 + 0];
			float y = data[i * 3 + 1];
			float z = data[i * 3 + 2];
			ret.emplace_back(glm::vec3(x, y, z));
		}

		return ret;
	}

	vector<glm::vec4> GltfLoader::getDataVec4(int accessorId)
	{
		vector<glm::vec4> ret;
		GltfAccessor &acc = accessors[accessorId];
		GltfBufferView &bv = bufferViews[acc.bufferViewId];
		float *data = (float *)getDataChunk(acc.bufferViewId, acc.byteOffset);
		for (int i = 0; i < acc.count; i++)
		{
			float x = data[i * 4 + 0];
			float y = data[i * 4 + 1];
			float z = data[i * 4 + 2];
			float w = data[i * 4 + 3];
			ret.emplace_back(glm::vec4(x, y, z, w));
		}

		return ret;
	}

	vector<glm::quat> GltfLoader::getDataQuat(int accessorId)
	{
		vector<glm::quat> ret;
		GltfAccessor &acc = accessors[accessorId];
		GltfBufferView &bv = bufferViews[acc.bufferViewId];
		float *data = (float *)getDataChunk(acc.bufferViewId, acc.byteOffset);
		for (int i = 0; i < acc.count; i++)
		{
			float x = data[i * 4 + 0];
			float y = data[i * 4 + 1];
			float z = data[i * 4 + 2];
			float w = data[i * 4 + 3];
			ret.emplace_back(glm::quat(w, x, y, z));
		}

		return ret;
	}

	Sprite *GltfLoader::getSprite(int meshId)
	{
		if (sprites.find(meshId) != sprites.end())
		{
			return sprites[meshId];
		}
		GltfMesh &gltfMesh = meshes[meshId];
		vector<Mesh *> meshes;

		for (GltfPrimitive &prim : gltfMesh.primitives)
		{
			vector<VertexAttribute *> attrs;
			DataBuffer *indexBuffer = nullptr;
			int indexType;

			for (auto &el : prim.attrAccessorIds)
			{
				string name = el.first;
				int accId = el.second;
				GltfAccessor &accessor = accessors[accId];
				GltfBufferView &bv = bufferViews[accessor.bufferViewId];

				VertexAttribute *attr = new VertexAttribute(name, accessor.type, accessor.componentType, false, bv.byteStride, accessor.byteOffset + bv.byteOffset);
				attrs.emplace_back(attr);

				DataBuffer *dataBuf = buffers[bv.bufferId].glBuffer;
				attr->setBuffer(dataBuf);
			}

			VertexArray *va = new VertexArray(attrs);
			va->create();

			Mesh *mesh = nullptr;
			if (prim.isIndexed)
			{
				GltfAccessor &indAccessor = accessors[prim.indAccessorId];
				GltfBufferView &bv = bufferViews[indAccessor.bufferViewId];
				mesh = new Mesh(va, buffers[bv.bufferId].glBuffer, indAccessor.byteOffset + bv.byteOffset, indAccessor.count, prim.mode, indAccessor.componentType, nullptr);
			}
			// don't know if unindexed rendering will ever come up in gltf
			// else
			// {
			// 	mesh = new Mesh(va, bv.byteOffset, count, prim.mode, nullptr);
			// }
			if (prim.materialId > -1)
			{
				mesh->setMaterial(materials[prim.materialId]);
			}
			meshes.emplace_back(mesh);
		}

		Sprite *sprite = new Sprite(meshes);
		sprites[meshId] = sprite;
		return sprite;
	}

	SceneGraphNode *GltfLoader::getNode(int id)
	{
		GltfNode &gltfNode = nodes[id];
		SceneGraphNode *node = new SceneGraphNode();
		node->setName(gltfNode.name);
		node->setTranslation(gltfNode.translation);
		node->setScale(gltfNode.scale);
		node->setRotation(gltfNode.rotation);
		if (gltfNode.meshId > -1)
		{
			Sprite *sprite = getSprite(gltfNode.meshId);
			node->setSprite(sprite);
			sprite->addInst(node);

			node->setBoundingBox(meshes[gltfNode.meshId].bbox);

			vector<INodeAnimation *> animations = getAnimations(id);
			for (INodeAnimation *a : animations)
			{
				node->addAnimation(a);
			}
		}
		if (gltfNode.extensions != nullptr)
		{
			if (gltfNode.extensions->contains("KHR_lights_punctual"))
			{
				int lightId = jser::getInt((*gltfNode.extensions)["KHR_lights_punctual"], "light");
				json &jLight = jRoot["extensions"]["KHR_lights_punctual"]["lights"][lightId];
				string type = jser::getString(jLight, "type");
				if (type == "point")
				{
					node->setSprite(lights[lightId], PTLIGHT_NODE);
				}
				else if (type == "directional")
				{
					node->setSprite(lights[lightId], DIRLIGHT_NODE);
				}
				else if (type == "spot")
				{
					node->setSprite(lights[lightId], SPOTLIGHT_NODE);
				}
			}
		}
		for (int i : gltfNode.childNodeIds)
		{
			SceneGraphNode *child = getNode(i);
			node->addChild(child);
		}

		return node;
	}

	vector<INodeAnimation *> GltfLoader::getAnimations(int nodeId)
	{
		vector<INodeAnimation *> v;
		GltfNode &node = nodes[nodeId];
		for (int animId : node.animationIds)
		{
			GltfAnimation &a = animations[animId];
			for (GltfChannel channel : a.channels)
			{
				if (channel.targetPath == "rotation")
				{
					RotationAnimation *anim = new RotationAnimation();
					GltfAnimSampler &sampler = a.samplers[channel.samplerId];
					std::vector<float> times = getDataFloat(sampler.inputAccessorId);
					vector<glm::quat> states = getDataQuat(sampler.outputAccessorId);
					anim->setFrameTimes(times);
					anim->setFrameStates(states);
					v.emplace_back(anim);
				}
			}
		}

		return v;
	}

	SceneGraph *GltfLoader::getSceneGraph(Scene *scene)
	{
		GltfScene s = scenes[defaultSceneId];
		SceneGraph *graph = new SceneGraph();
		SceneGraphNode *root = graph->getRootNode();
		for (int id : s.nodeIds)
		{
			SceneGraphNode *node = getNode(id);
			root->addChild(node);
		}

		if (scene != nullptr)
		{
			for (int id = 0; id < sprites.size(); id++)
			{
				scene->addSprite(sprites[id]);
			}
		}
		return graph;
	}

	SceneGraph *GltfLoader::getSceneGraph(Scene *scene, SceneGraphNode *root)
	{
		GltfScene s = scenes[defaultSceneId];
		SceneGraph *graph = new SceneGraph();
		for (int id : s.nodeIds)
		{
			SceneGraphNode *node = getNode(id);
			root->addChild(node);
		}

		if (scene != nullptr)
		{
			for (int id = 0; id < sprites.size(); id++)
			{
				scene->addSprite(sprites[id]);
			}
		}
		return graph;
	}

	SceneGraphNode *GltfLoader::getSceneRootNode(int sceneId)
	{
		return getNode(scenes[sceneId].nodeIds[0]);
	}
}