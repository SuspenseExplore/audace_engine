#ifndef AU_BITMAPFONT_H
#define AU_BITMAPFONT_H

#include <map>
#include "FileLoader.h"
#include "renderer/ShaderProgram.h"
#include "renderer/Texture2d.h"
#include "renderer/Shapes.h"
#include "renderer/Mesh.h"
#include "ft2build.h"
#include FT_FREETYPE_H

namespace Audace
{
	struct Character {
		Texture2d *texture;
		glm::vec2 size;
		glm::vec2 bearing;
		int advance;
	};

	class BitmapFont
	{
		ShaderProgram *shader;
		std::map<char, Character> characters;
		Mesh *quadMesh;

	public:
		BitmapFont(FileLoader* loader, std::string filepath);
		void renderText(std::string text, glm::vec4 color = glm::vec4(1, 1, 1, 1));

		ShaderProgram *getShader() {return shader;}
		Character getChar(char c) {return characters[c];}
	};
}

#endif