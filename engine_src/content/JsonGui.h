#ifndef AU_JSONGUI_H
#define AU_JSONGUI_H

#include <string>
#include <map>
#include <functional>
#include "imgui.h"
#include "glm/glm.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace Audace
{
	class SceneGraph;
	class SceneGraphNode;

	union Binding
	{
		bool* boolean;
		glm::vec4* float4;
		SceneGraph* sceneGraph;
		std::function<void(SceneGraphNode*)>* graphNodeFn;
	};

	class JsonGui
	{
		json jcontent;
		std::map<std::string, Binding> bindings;

	public:
		JsonGui(json content);

		void addBinding(std::string name, bool* b);
		void addBinding(std::string name, glm::vec4* v);
		void addBinding(std::string name, SceneGraph* sg);
		void addBinding(std::string name, std::function<void(SceneGraphNode*)>* fn);

		void render();
		void render(json& j);
		void window(json& j);
		void tabBar(json& j);
		void tabItem(json& j);
		void dragFloat4(json& j);
		void checkbox(json& j);
		void separator();
		void sceneGraphTree(json& j);
		void sceneGraphNode(json& j, SceneGraphNode* node, const std::string& path);
		void sameLine(json& j);
		void button(json& j, SceneGraphNode* node);
	};
}

#endif