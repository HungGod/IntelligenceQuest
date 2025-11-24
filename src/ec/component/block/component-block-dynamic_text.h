#pragma once
#include "ec/component/block/component-block.h"
#include "ec/component/material/component-material.h"
#include "text.h"
#include "ec/component/src/component-src-bm_font.h"

namespace Block
{

	class DynamicTemp : public IBlock
	{
		Component::ValTemplate<int>* temp;
		int em_width;
		IMaterial* font_material;
		Text text;
		float val;
	public:

		void init(nlohmann::json json, Entity* game)
		{
			temp = game->get_nested_component<Component::ValTemplate<int>>(json["val"]);
			val = temp->val;
			Entity* font = game->get_nested_child(json["font"]);
			font_material = font->get_component<IMaterial>("material");
			float scale = json["scale"];
			IBlock::init(json, game);
			text.init(font, scale);
		}

		void draw(glm::vec2 position, Component::Renderer* renderer)
		{
			if (val != temp->val)
			{
				val = temp->val;
				text.write(std::to_string(val), position);
			}

			for (auto i = 0; i < text.glyphs.size(); ++i)
			{
				glm::vec4 dest{ position.x + text.dests[i].x, position.y + text.dests[i].y, 
					position.x + text.dests[i].w, position.y + text.dests[i].z };
				text.glyphs[i]->draw(dest, font_material, renderer);
			}
		}

		std::string get_id() override { return "block-dynamic_text"; }
	};
}