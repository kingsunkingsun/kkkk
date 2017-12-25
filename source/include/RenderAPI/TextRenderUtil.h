#pragma once

#include <ft2build.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <exception>
#include <map>
#include FT_FREETYPE_H

/* 
提供接口，方便地调出对应一个char的mesh、texture。
提供char，返回对应的VAO、Texture ID
*/

class TextRenderUtil {
private:
	FT_Library ft;
	FT_Face face;
	GLuint VAO, VBO;
public:
	struct Character {
		GLuint     TextureID;  // ID handle of the glyph texture
		glm::ivec2 Size;       // Size of glyph
		glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
		GLuint     Advance;    // Offset to advance to next glyph
	};
	std::map<GLchar, Character> Characters;

	static TextRenderUtil& GetInstance() {
		static TextRenderUtil instance;
		return instance;
	}
	TextRenderUtil() {
		FT_Library ft;
		if (FT_Init_FreeType(&ft))
			throw std::exception("ERROR::FREETYPE: Could not init FreeType Library");

		FT_Face face;
		if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
			throw std::exception("ERROR::FREETYPE: Failed to load font");

		FT_Set_Pixel_Sizes(face, 0, 48);

		if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
			throw std::exception("ERROR::FREETYTPE: Failed to load Glyph");

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

		for (GLubyte c = 0; c < 128; c++) {
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
				throw std::exception("ERROR::FREETYTPE: Failed to load Glyph");
				continue;
			}
			// Generate texture
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// Set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// Now store character for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				face->glyph->advance.x
			};
			Characters.insert(std::pair<GLchar, Character>(c, character));
		}
		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 5, NULL, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	GLuint GetVAO() {
		return VAO;
	}
	GLuint GetTextureIDForCharacter(char c, GLfloat* advance, GLfloat scale = 1.0f) {
		Character ch = Characters[c];

		GLfloat xpos = ch.Bearing.x * scale;
		GLfloat ypos = -(ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][5] = {
			{ xpos,     ypos + h,  0.0,  0.0, 0.0 },
			{ xpos,     ypos,      0.0,  0.0, 1.0 },
			{ xpos + w, ypos,      0.0,  1.0, 1.0 },

			{ xpos,     ypos + h,  0.0,  0.0, 0.0 },
			{ xpos + w, ypos,      0.0,  1.0, 1.0 },
			{ xpos + w, ypos + h,  0.0,  1.0, 0.0 }
		};
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		*advance = (ch.Advance >> 6 )* scale;
		return ch.TextureID;
	}
};
