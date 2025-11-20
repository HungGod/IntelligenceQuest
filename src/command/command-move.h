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
	class Move : public ICommand
	{
		Component::Position* position_;
		Src::Transform* src_temp_;
		Component::AnimationMap* anim_map_;
		Component::Float* delta_time_;
		glm::vec2 end_position_;
		float move_speed_;
		std::string end_direction_;
		System::Camera* camera_;
		float anim_speed_;

	public:
		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
			position_ = game->get_nested_component<Component::Position>(json["position"]);
			anim_map_ = game->get_nested_component<Component::AnimationMap>(json["animation_map"]);
			src_temp_ = game->get_nested_component<Src::Transform>(json["src_temp"]);
			delta_time_ = game->get_component<Component::Float>("delta_time");
			anim_speed_ = json["anim_speed"];
			if (json.contains("camera"))
				camera_ = game->get_nested_component<System::Camera>(json["camera"]);
			else
				camera_ = nullptr;
			
			if (json["end_position"].contains("position_map"))
			{
				auto position_map = game->get_nested_component<Component::PositionMap>(json["end_position"]["position_map"]);
				end_position_ = position_map->at(json["end_position"]["id"])[json["end_position"]["num"]];
			}
			else
			{
				end_position_.x = json["end_position"]["x"];
				end_position_.y = json["end_position"]["y"];
			}

			// Final facing direction sprite when the move completes.
			end_direction_ = json["end_direction"];
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
				src_temp_->src = (*anim_map_)[end_direction_][0];
				return;
			}

			// Normalize the direction and update position.
			glm::vec2 direction = to_target / distance_remaining;
			current += direction * step;
			position_->x = current.x;
			position_->y = current.y;

			// Determine the proper walking animation based on the movement direction.
			std::vector<ISrc*> anim;
			if (std::abs(direction.y) > std::abs(direction.x))
			{
				anim = (direction.y > 0) ? (*anim_map_)["walk_down"]
					: (*anim_map_)["walk_up"];
			}
			else
			{
				anim = (direction.x > 0) ? (*anim_map_)["walk_right"]
					: (*anim_map_)["walk_left"];
			}

			// Choose an animation frame based on the current time.

			auto time = static_cast<float>(glfwGetTime());
			auto anim_pos = static_cast<std::size_t>(time * anim_speed_) % anim.size(); // find next frame to animate
			src_temp_->src = anim[anim_pos];

			if (camera_)
				camera_->execute();

			pathway->next(this);
		}

		std::string to_string() override { return "move"; }
	};
}