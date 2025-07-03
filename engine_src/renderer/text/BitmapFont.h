#ifndef AU_BITMAPFONT_H
#define AU_BITMAPFONT_H

#include <map>
#include "glm/glm.hpp"
#include "ft2build.h"
#include FT_FREETYPE_H

namespace Audace
{
	class IFileAccess;
	class Texture2d;
	class ShaderProgram;
	class Mesh;

	struct Character {
		Texture2d* texture;
		glm::vec2 size;
		glm::vec2 bearing;
		int advance;
	};

	class BitmapFont
	{
		ShaderProgram* shader;
		std::map<char, Character> characters;
		Mesh* quadMesh;

	public:
		BitmapFont(IFileAccess* loader, std::string filepath);
		void renderText(std::string text, glm::vec4 color = glm::vec4(1, 1, 1, 1));

		ShaderProgram* getShader() { return shader; }
		Character getChar(char c) { return characters[c]; }
	};
}

#endif