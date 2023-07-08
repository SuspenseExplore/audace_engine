#include <sstream>
#include "AssetStore.h"

namespace Audace
{
	FileLoader *AssetStore::fileLoader;
	std::map<std::string, ShaderProgram *> AssetStore::shaders;

	void AssetStore::init(FileLoader* loader) {
		fileLoader = loader;
		getShader("AU_simple_text");
		getShader("AU_simple_billboard");
	}

	ShaderProgram *AssetStore::getShader(const std::string &name)
	{
		if (shaders.find(name) == shaders.end())
		{
			std::stringstream ss;
			ss << "shaders/" << name << "/vs.glsl";
			std::string vs = fileLoader->textFileToString(ss.str());
			ss.str(std::string());
			ss << "shaders/" << name << "/fs.glsl";
			std::string fs = fileLoader->textFileToString(ss.str());
			ShaderProgram *shaderProgram = new Audace::ShaderProgram(vs, fs);
			shaderProgram->create();
			shaders[name] = shaderProgram;
			return shaderProgram;
		}
		else
		{
			return shaders[name];
		}
	}
}