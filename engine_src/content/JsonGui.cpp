#include "JsonGui.h"
#include "JsonSerializer.h"
#include "scene/graph/SceneGraph.h"
#include "scene/graph/SceneGraphNode.h"
#include "glm/gtc/type_ptr.hpp"

#define NAME "name"
#define TYPE "type"
#define CHILDREN "children"
#define WINDOW "window"
#define TAB_BAR "tab_bar"
#define TAB_ITEMS "tab_items"
#define DRAG_FLOAT "drag_float"
#define DRAG_FLOAT3 "drag_float3"
#define DRAG_FLOAT4 "drag_float4"
#define LIGHT_COLOR "light_color"
#define CHECKBOX "checkbox"
#define SEPARATOR "separator"
#define TREE "tree"
#define TREE_NODE "tree_node"
#define BUTTON "button"
#define SAME_LINE "same_line"
#define NEW_LINE "new_line"
#define X_OFFSET "x_offset"
#define NODE_TYPE "node_type"

#define SPEED "speed"
#define MIN "min"
#define MAX "max"

namespace Audace
{
	typedef JsonSerializer jser;

	JsonGui::JsonGui(json content) : jcontent(content)
	{
		emptyBindingFloats[0] = 0;
		emptyBindingFloats[1] = 0;
		emptyBindingFloats[2] = 0;
		emptyBindingFloats[3] = 0;
	}

	void JsonGui::addBinding(std::string name, bool* b)
	{
		bindings[name].boolean = b;
	}

	void JsonGui::addBinding(std::string name, int* i)
	{
		bindings[name].integer = i;
	}

	void JsonGui::addBinding(std::string name, float* f)
	{
		bindings[name].float1 = f;
	}

	void JsonGui::addBinding(std::string name, glm::vec3* v)
	{
		bindings[name].float3 = v;
	}

	void JsonGui::addBinding(std::string name, glm::vec4* v)
	{
		bindings[name].float4 = v;
	}

	void JsonGui::addBinding(std::string name, SceneGraph* sg)
	{
		bindings[name].sceneGraph = sg;
	}

	void JsonGui::addBinding(std::string name, std::function<void(SceneGraphNode*)>* sgnFn)
	{
		bindings[name].graphNodeFn = sgnFn;
	}

	void JsonGui::render()
	{
		render(jcontent);
	}

	void JsonGui::render(json& j)
	{
		std::string type = jser::getString(j, TYPE);
		if (type == WINDOW)
		{
			window(j);
		}
		else if (type == TAB_BAR)
		{
			tabBar(j);
		}
		else if (type == DRAG_FLOAT)
		{
			dragFloat(j);
		}
		else if (type == DRAG_FLOAT3)
		{
			dragFloat3(j);
		}
		else if (type == DRAG_FLOAT4)
		{
			dragFloat4(j);
		}
		else if (type == LIGHT_COLOR)
		{
			lightColor(j);
		}
		else if (type == CHECKBOX)
		{
			checkbox(j);
		}
		else if (type == SEPARATOR)
		{
			separator();
		}
		else if (type == TREE && jser::getString(j, NAME) == "scene_graph")
		{
			sceneGraphTree(j);
		}
		else if (type == SAME_LINE)
		{
			sameLine(j);
		}
		else if (type == NEW_LINE)
		{
			ImGui::NewLine();
		}
	}

	void JsonGui::window(json& j)
	{
		ImGui::Begin(jser::getString(j, NAME).c_str());

		for (json& c : j[CHILDREN])
		{
			render(c);
		}

		ImGui::End();
	}

	void JsonGui::tabBar(json& j)
	{
		if (ImGui::BeginTabBar(jser::getString(j, NAME).c_str()))
		{
			for (json& c : j[TAB_ITEMS])
			{
				tabItem(c);
			}

			ImGui::EndTabBar();
		}
	}

	void JsonGui::tabItem(json& j)
	{
		bool vis = true;
		if (j.contains("visibility") && j["visibility"].contains("equals"))
		{
			int comp = jser::getInt(j["visibility"]["equals"], NODE_TYPE);
			vis = comp == *bindings[NODE_TYPE].integer;
		}
		if (vis)
		{
			if (ImGui::BeginTabItem(jser::getString(j, NAME).c_str()))
			{
				for (json& c : j[CHILDREN])
				{
					render(c);
				}

				ImGui::EndTabItem();
			}
		}
	}

	void JsonGui::dragFloat(json& j)
	{
		std::string name = jser::getString(j, NAME);
		float s = 1;
		float min = -FLT_MAX;
		float max = FLT_MAX;
		if (j.contains(SPEED))
		{
			s = jser::getFloat(j, SPEED);
		}
		if (j.contains(MIN))
		{
			min = jser::getFloat(j, MIN);
		}
		if (j.contains(MAX))
		{
			max = jser::getFloat(j, MAX);
		}
		ImGui::DragFloat(name.c_str(), bindings[name].float1, s, min, max);
	}

	void JsonGui::dragFloat3(json& j)
	{
		std::string name = jser::getString(j, NAME);
		float s = 1;
		float min = -FLT_MAX;
		float max = FLT_MAX;
		if (j.contains(SPEED))
		{
			s = jser::getFloat(j, SPEED);
		}
		if (j.contains(MIN))
		{
			min = jser::getFloat(j, MIN);
		}
		if (j.contains(MAX))
		{
			max = jser::getFloat(j, MAX);
		}
		ImGui::DragFloat3(name.c_str(), glm::value_ptr(*bindings[name].float3), s, min, max);
	}

	void JsonGui::dragFloat4(json& j)
	{
		std::string name = jser::getString(j, NAME);
		float s = 1;
		float min = -FLT_MAX;
		float max = FLT_MAX;
		if (j.contains(SPEED))
		{
			s = jser::getFloat(j, SPEED);
		}
		if (j.contains(MIN))
		{
			min = jser::getFloat(j, MIN);
		}
		if (j.contains(MAX))
		{
			max = jser::getFloat(j, MAX);
		}
		ImGui::ColorEdit4(name.c_str(), glm::value_ptr(*bindings[name].float4));
	}

	void JsonGui::lightColor(json& j)
	{
		std::string name = jser::getString(j, NAME);
		ImGui::ColorEdit3(name.c_str(), glm::value_ptr(*bindings[name].float4));
		ImGui::DragFloat("Intensity", &(*bindings[name].float4).a, 0.01, 0, 1000);
	}

	void JsonGui::checkbox(json& j)
	{
		std::string name = jser::getString(j, NAME);
		ImGui::Checkbox(name.c_str(), bindings[name].boolean);
	}

	void JsonGui::separator()
	{
		ImGui::Separator();
	}

	void JsonGui::sceneGraphTree(json& j)
	{
		const std::string& name = jser::getString(j, NAME);
		SceneGraph* graph = bindings[name].sceneGraph;
		sceneGraphNode(j[TREE_NODE], graph->getRootNode(), "0");
	}

	void JsonGui::sceneGraphNode(json& j, SceneGraphNode* node, const std::string& path)
	{
		std::string n = (node->getName().length() > 0) ? node->getName() : path;
		ImGui::PushID(path.c_str());
		bool r = ImGui::TreeNode(n.c_str());
		json& jchildren = j[CHILDREN];
		for (auto& jchild : jchildren)
		{
			if (jser::getString(jchild, TYPE) == BUTTON)
			{
				button(jchild, node);
			}
			else
			{
				render(jchild);
			}
		}

		// if this node is open
		if (r)
		{
			std::vector<SceneGraphNode*> children = node->getChildren();
			for (int i = 0; i < children.size(); i++)
			{
				sceneGraphNode(j, children[i], path + "/" + std::to_string(i));
			}
			// pop the tree when all children at this level are done
			ImGui::TreePop();
		}
		ImGui::PopID();
	}

	void JsonGui::button(json& j, SceneGraphNode* node)
	{
		std::string name = jser::getString(j, NAME);
		if (ImGui::Button(name.c_str()))
		{
			(*(bindings[name].graphNodeFn))(node);
		}
	}

	void JsonGui::sameLine(json& j)
	{
		if (j.contains(X_OFFSET))
		{
			float x = jser::getFloat(j, X_OFFSET);
			ImGui::SameLine(x);
		}
		else
		{
			ImGui::SameLine();
		}
	}
}