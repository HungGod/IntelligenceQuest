#pragma once
#include "ec/component/system/component-system.h"
#include "templates.h"
#include "ec/component/controller/component-controller.h"

namespace System
{
	constexpr auto ONE_OVER_SQRT_TWO = 0.70710678118f;
	class Move : public ISystem
	{
		Component::Position* position_;
		Component::Velocity* velocity_;
		Component::Float* delta_time_, *speed_;
		Component::Direction* direction_;
	public:
		Move()
			: position_(nullptr), direction_(nullptr), delta_time_(nullptr), speed_(nullptr), velocity_(nullptr)
		{}

		void init(nlohmann::json json, Entity* game) override
		{
			position_ = game->get_nested_component<Component::Position>(json["position"]);
			direction_ = game->get_nested_component<Component::Direction>(json["direction"]);
			speed_ = game->get_nested_component<Component::Float>(json["speed"]);
			velocity_ = game->get_nested_component<Component::Velocity>(json["velocity"]);
			delta_time_ = game->get_component<Component::Float>("delta_time");
		}

		void execute() override
		{
			auto dir = (*direction_);

			if (direction_->y != 0.0f && direction_->x != 0.0f)
				dir *= ONE_OVER_SQRT_TWO;
			
			velocity_->x += dir.x * speed_->val;
			velocity_->y += dir.y * speed_->val;
			position_->x += velocity_->x * delta_time_->val;
			position_->y += velocity_->y * delta_time_->val;
		}

		std::string get_id() override { return "system-move"; }
	};
}
