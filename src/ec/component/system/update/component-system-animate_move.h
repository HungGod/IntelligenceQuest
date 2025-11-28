#pragma once
#pragma once
#include "ec/component/system/component-system.h"
#include "ec/component/component-template.h"
#include "ec/component/controller/component-controller.h"
#include <glm/vec2.hpp>
#include "ec/component/component-animation_map.h"
#include "ec/entity.h"
#include "ec/component/component-animation.h"

/*
Animates object to play walk and idle animations on controller move events

@author David Wadsworth
*/

namespace System {
	class AnimateMove : public ISystem
	{
		Component::AnimationMap* animation_map_;
		Component::Animation* animation_;
		Component::Vector2D* direction_, *prev_;
	public:
		AnimateMove()
			: animation_map_(nullptr), animation_(nullptr), direction_(nullptr), prev_(nullptr)
		{}

		void init(nlohmann::json json, Entity* game) override
		{
			direction_ = game->get_nested_component<Component::Vector2D>(json["direction"]);
			animation_map_ = game->get_nested_component<Component::AnimationMap>(json["animation_map"]);
			animation_ = game->get_nested_component<Component::Animation>(json["animation"]);
			prev_ = game->get_nested_component<Component::Vector2D>(json["prev_direction"]);
		}

		void execute() override
		{
			// if curr.x and curr.y is not 0 then its moving, else its idling
			if (direction_->x || direction_->y)
			{
				if (direction_->x > 0)
				{
					if (direction_->y > 0)
						animation_->val = &(*animation_map_)["walk_down_right"];
					else if (direction_->y < 0)
						animation_->val = &(*animation_map_)["walk_up_right"];
					else
						animation_->val = &(*animation_map_)["walk_right"];
				}
				else if (direction_->x < 0)
				{
					if (direction_->y > 0)
						animation_->val = &(*animation_map_)["walk_down_left"];
					else if (direction_->y < 0)
						animation_->val = &(*animation_map_)["walk_up_left"];
					else
						animation_->val = &(*animation_map_)["walk_left"];
				}
				else if (direction_->y > 0)
					animation_->val = &(*animation_map_)["walk_down"];
				else if (direction_->y < 0)
					animation_->val = &(*animation_map_)["walk_up"];

				prev_->x = direction_->x;
				prev_->y = direction_->y;
			}
			else
			{
				if (prev_->x > 0)
				{
					if (prev_->y > 0)
						animation_->val = &(*animation_map_)["idle_down_right"];
					else if (prev_->y < 0)
						animation_->val = &(*animation_map_)["idle_up_right"];
					else
						animation_->val = &(*animation_map_)["idle_right"];
				}
				else if (prev_->x < 0)
				{
					if (prev_->y > 0)
						animation_->val = &(*animation_map_)["idle_down_left"];
					else if (prev_->y < 0)
						animation_->val = &(*animation_map_)["idle_up_left"];
					else
						animation_->val = &(*animation_map_)["idle_left"];
				}
				else if (prev_->y > 0)
					animation_->val = &(*animation_map_)["idle_down"];
				else if (prev_->y < 0)
					animation_->val = &(*animation_map_)["idle_up"];
			}
		}
		std::string get_id() override { return "system-animate_move"; }
	};
}
