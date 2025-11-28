#pragma once
#include "ec/component/block/component-block.h"
#include "ec/component/src/component-src-bm_font.h"
#include "text.h"

namespace Block
{
	class Words : public IBlock
	{
	public:
		IMaterial* material;
		Text text;

		void init(nlohmann::json json, Entity* game)
		{
			Entity* font = game->get_nested_child(json["font"]);
			material = font->get_component<IMaterial>("material");
			Component::ValTemplate<float>* scale = game->get_nested_component<Component::ValTemplate<float>>(json["scale"]);
			text.init(font, scale->val);

			if (json.contains("message"))
			{
				std::string message = json["message"];
				text.write(message, glm::vec2(0));
			}

			IBlock::init(json, game);
		}

		void draw(glm::vec2 position, Component::Renderer* renderer) override
		{
			for (auto i = 0; i < text.dests.size(); ++i)
			{
				glm::vec4 temp_dest = text.dests[i];
				temp_dest.x += position.x;
				temp_dest.y += position.y;
				temp_dest.z += position.x;
				temp_dest.w += position.y;
				text.glyphs[i]->draw(temp_dest, material, renderer);
			}

		}
		std::string get_id() override { return "block-text.h"; }
	};
}