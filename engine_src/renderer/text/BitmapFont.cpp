#include "AuLogger.h"
#include "BitmapFont.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Audace
{
	BitmapFont::BitmapFont(FileLoader *loader, std::string filepath)
	{
		std::string vs = loader->textFileToString("shaders/color/vs.glsl");
		std::string fs = loader->textFileToString("shaders/color/fs.glsl");
		shader = new Audace::ShaderProgram(vs, fs);
		shader->create();

		quadMesh = Shapes::squarePositions();

		FT_Library freetype;
		if (FT_Init_FreeType(&freetype))
		{
			AU_ENGINE_LOG_ERROR("Freetype failed to initialize");
			return;
		}

		ByteBuffer *buffer = loader->readFileToBuffer("fonts/arial.ttf");
		FT_Face face;
		if (FT_New_Memory_Face(freetype, (unsigned char *)buffer->getBuffer(), buffer->getLength(), 0, &face))
		{
			AU_ENGINE_LOG_ERROR("Freetype failed to load font: {}", "arial.ttf");
			return;
		}

		FT_Set_Pixel_Sizes(face, 0, 48);
		for (unsigned char c = 0; c < 128; c++)
		{
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				AU_ENGINE_LOG_ERROR("Freetype failed to load glyph: {}", c);
				continue;
			}
			Character ch;

			ImageData img(face->glyph->bitmap.buffer, face->glyph->bitmap.width, face->glyph->bitmap.rows, GL_ALPHA);
			ch.texture = new Texture2d(img);
			ch.texture->create();
			ch.size = glm::vec2(img.width, img.height);
			ch.bearing = glm::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
			ch.advance = face->glyph->advance.x;
			characters[c] = ch;
		}
	}

	void BitmapFont::renderText(std::string text)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		shader->bind();
		int x = -500;
		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++) {
			Character ch = characters[*c];
			glm::mat4 worldMat = glm::ortho(-640.0f, 640.0f, 360.0f, -360.0f);
			worldMat = glm::translate(worldMat, glm::vec3(x + ch.bearing.x, -ch.bearing.y, 0.0f));
			worldMat = glm::scale(worldMat, glm::vec3(ch.size.x, ch.size.y, 1.0f));
			
			shader->setUniformMat4("txMat", glm::value_ptr(worldMat));
			shader->setUniformInt("tex1", 0);
			ch.texture->bind(0);
			quadMesh->render();

			x += (ch.advance >> 6);
		}
	}
}