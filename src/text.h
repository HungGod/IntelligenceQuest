#pragma once
#include "ec/component/src/component-src-bm_font.h"
#include "ec/component/component-template.h"
#include "ec/entity.h"
#include "rect.h"

class Text
{
public:
	std::vector<Src::BMFont*> glyphs;
	std::vector<glm::vec4> dests;
	Entity* font;
	IMaterial* material;
	int size;
	float scale;

	void write(std::string text, glm::vec2 position)
	{
		glyphs.clear();
		dests.clear();
		size = 0;

		float space = static_cast<float>(font->get_component<Component::ValTemplate<int>>("space")->val);
		float line_h = static_cast<float>(font->get_component<Component::ValTemplate<int>>("line_h")->val);

		int prev_char = -1;

		glm::vec2 current_pos = position;
		// build src_dests to use for rendering
		for (int ch : text)
		{
			switch (ch)
			{
			case ' ':
				current_pos.x += space * scale;
				prev_char = -1;
				break;
			case '\t':
				current_pos.x += space * 4 * scale;
				prev_char = -1;
				break;
			case '\n':
				current_pos.x = position.x;
				current_pos.y += line_h * scale;
				prev_char = -1;
				break;
			default:
				auto bitmap_glyph = font->get_component<Src::BMFont>(ch);
				glm::vec4 dest{};
				float kerning = bitmap_glyph->kerning.count(prev_char)
					? bitmap_glyph->kerning[prev_char] * scale
					: 0;
				dest.x = current_pos.x + bitmap_glyph->offset.x * scale + kerning;
				dest.y = current_pos.y + bitmap_glyph->offset.y * scale;
				dest.z = dest.x + bitmap_glyph->offset.w * scale;
				dest.w = dest.y + bitmap_glyph->offset.h * scale;
				current_pos.x += bitmap_glyph->advance * scale;
				prev_char = bitmap_glyph->glyph;
				glyphs.push_back(bitmap_glyph);
				dests.push_back(dest);
				size += bitmap_glyph->offset.w * scale;
				break;
			}
		}
	}

	void write(std::string text, glm::vec2 position, float width)
	{
		glyphs.clear();
		dests.clear();
		size = width;

		float space = static_cast<float>(font->get_component<Component::ValTemplate<int>>("space")->val);
		float line_h = static_cast<float>(font->get_component<Component::ValTemplate<int>>("line_h")->val);

		int prev_char = -1;

		glm::vec2 current_pos = position;
		
		std::vector<int> curr_word;
		for (int ch : text)
		{
			switch (ch)
			{
			case ' ':
				current_pos.x += space * scale;
				curr_word.clear();
				prev_char = -1;
				break;
			case '\t':
				current_pos.x += space * 4 * scale;
				curr_word.clear();
				prev_char = -1;
				break;
			case '\n':
				current_pos.x = position.x;
				current_pos.y += line_h * scale;
				curr_word.clear();
				prev_char = -1;
				break;
			default:
				auto bitmap_glyph = font->get_component<Src::BMFont>(ch);
				glm::vec4 dest{};
				float kerning = bitmap_glyph->kerning.count(prev_char)
					? bitmap_glyph->kerning[prev_char] * scale
					: 0;
				dest.x = current_pos.x + bitmap_glyph->offset.x * scale + kerning;
				dest.y = current_pos.y + bitmap_glyph->offset.y * scale;
				dest.z = dest.x + bitmap_glyph->offset.w * scale;
				dest.w = dest.y + bitmap_glyph->offset.h * scale;
				current_pos.x += bitmap_glyph->advance * scale;

				prev_char = bitmap_glyph->glyph;
				glyphs.push_back(bitmap_glyph);
				dests.push_back(dest);
				curr_word.push_back(dests.size() - 1);
				if (current_pos.x > position.x + width)
				{
					auto offset_x = dests[curr_word.front()].x;
					for (auto w : curr_word)
					{
						dests[w].x = dests[w].x - offset_x + position.x;
						dests[w].z = dests[w].z - offset_x + position.x;
						dests[w].y += line_h * scale;
						dests[w].w += line_h * scale;
					}
					current_pos.x = dests[curr_word.back()].x + bitmap_glyph->advance * scale;
					current_pos.y += line_h * scale;
				}
				break;
			}
		}
	}

	void draw(Component::Renderer* renderer)
	{
		for (int i = 0; i < dests.size(); ++i)
			glyphs[i]->draw(dests[i], material, renderer);
	}

	void draw(glm::vec2 position, Component::Renderer* renderer)
	{
		for (auto i = 0; i < dests.size(); ++i)
		{
			glm::vec4 temp_dest = dests[i];
			temp_dest.x += position.x;
			temp_dest.y += position.y;
			temp_dest.z += position.x;
			temp_dest.w += position.y;

			glyphs[i]->draw(temp_dest, material, renderer);
		}
	}

	void init(Entity* font, float scale)
	{
		this->font = font;
		this->material = font->get_component<IMaterial>("material");
		this->scale = scale;
	}
};