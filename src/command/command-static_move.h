#pragma once
#include "command/command.h"
#include "ec/component/component-position_map.h"
#include "ec/component/component-animation_map.h"
#include "ec/component/component-vec2.h"
#include "ec/component/component-pathway.h"
#include <GLFW/glfw3.h>
#include "ec/component/src-transform/component-src-transform.h"
#include <glm/vec2.hpp>
#include <glm/geometric.hpp>
#include "ec/component/system-camera/component-system-camera.h"

namespace Command
{
	class StaticMove : public ICommand
	{
		Component::Position* position_;
		Component::Float* delta_time_;
		glm::vec2 end_position_;
		float move_speed_;

	public:
		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
			position_ = game->get_nested_component<Component::Position>(json["position"]);
			delta_time_ = game->get_component<Component::Float>("delta_time");
			end_position_.x = json["end_position"]["x"];
			end_position_.y = json["end_position"]["y"];
			
			move_speed_ = json["move_speed"];
		}

		void execute(Entity* game, Component::Pathway* pathway)
		{
			// Create a vector from the current position.
			glm::vec2 current(position_->x, position_->y);
			// Compute the vector towards the target.
			glm::vec2 to_target = end_position_ - current;
			float distance_remaining = glm::length(to_target);
			// Calculate the movement step based on speed and delta time.
			float step = move_speed_ * delta_time_->val;

			// If we're close enough, snap to the destination.
			if (distance_remaining <= step)
			{
				position_->x = end_position_.x;
				position_->y = end_position_.y;
				return;
			}

			// Normalize the direction and update position.
			glm::vec2 direction = to_target / distance_remaining;
			current += direction * step;
			position_->x = current.x;
			position_->y = current.y;

			pathway->next(this);
		}

		std::string to_string() override { return "static_move"; }
	};
}