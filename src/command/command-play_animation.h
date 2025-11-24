#pragma once
#include "command/command.h"
#include "ec/component/component-animation_map.h"
#include "ec/component/component-animation.h"
#include "ec/component/src/component-src-transform.h"
#include "ec/component/component-pathway.h"
#include "ec/entity.h"
#include <GLFW/glfw3.h>

namespace Command
{
	class PlayAnimation : public ICommand
	{
		std::vector<ISrc*> animation_;
		Src::Transform* src_temp_;
		int pos_;
		float play_speed_;
		double timestamp;
		Component::Float* delta_time_;
	public:

		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
			auto animation_map = game->get_nested_component<Component::AnimationMap>(json["animation_map"]);
			std::string animation_id = json["animation_id"];

			animation_ = animation_map->at(animation_id);
			src_temp_ = game->get_nested_component<Src::Transform>(json["src_temp"]);
			play_speed_ = json["play_speed"];
			delta_time_ = game->get_component<Component::Float>("delta_time");
			pos_ = 0;
			timestamp = 0;
		}


		void execute(Entity* game, Component::Pathway* pathway) override
		{
			if (pos_ >= animation_.size())
				return;

			auto curr_time = glfwGetTime();
			if (timestamp + play_speed_ * delta_time_->val < curr_time)
			{
				timestamp = curr_time;
				src_temp_->src = animation_[pos_++];
			}

			pathway->next(this);
		}
		std::string to_string() override { return "play_animation"; }
	};
}