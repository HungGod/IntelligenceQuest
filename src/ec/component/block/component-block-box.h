#pragma once
#include "ec/component/block/component-block.h"
#include "ec/component/component-box.h"
#include "ec/component/component-template.h"
#include "templates.h"

namespace Block
{
	class Box : public IBlock
	{
		Component::Float* scale_;
		Component::Box* box_;
		Component::Box::BoxContainer box_container_;
	public:

		void init(nlohmann::json json, Entity* game)
		{
			box_ = game->get_nested_component<Component::Box>(json["box"]);
			scale_ = game->get_nested_component<Component::Float>(json["scale"]);
			change_arrow(true, false);
		}

		void change_arrow(bool left, bool no_speech_arrow)
		{
			box_container_ = box_->create_box(left, no_speech_arrow);
			box_container_.arrow_pos.x *= scale_->val;
			box_container_.arrow_pos.y *= scale_->val;
		}
		
		glm::vec2 get_arrow_position() { return box_container_.arrow_pos; }

		void draw(glm::vec2 position, Component::Renderer* renderer) override
		{
			for (auto i = 0; i < box_container_.srcs.size(); ++i)
			{
				for (auto j = 0; j < box_container_.srcs[i].size(); ++j)
				{
					if (box_container_.srcs[i][j] != nullptr)
					{
						box_container_.srcs[i][j]->draw(
							glm::vec2(
							j * box_container_.tile_size * scale_->val + position.x,
							i * box_container_.tile_size * scale_->val + position.y
							),
							scale_->val, box_container_.material, renderer);

					}
				}
			}
		}
		std::string get_id() override { return "block-box"; }
	};
}

