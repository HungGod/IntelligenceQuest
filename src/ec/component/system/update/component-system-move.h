#pragma once
#include "ec/component/system/component-system.h"
#include "ec/component/controller/component-controller.h"

namespace System
{
	constexpr auto ONE_OVER_SQRT_TWO = 0.70710678118f;
	class Move : public ISystem
	{
		Component::Vector2D* velocity_;
		Component::ValTemplate<float>* delta_time_, *speed_;
		Component::Vector2D* direction_;
	public:
		Move()
			: direction_(nullptr), delta_time_(nullptr), speed_(nullptr), velocity_(nullptr)
		{}

		void init(nlohmann::json json, Entity* game) override
		{
			direction_ = game->get_nested_component<Component::Vector2D>(json["direction"]);
			speed_ = game->get_nested_component<Component::ValTemplate<float>>(json["speed"]);
			velocity_ = game->get_nested_component<Component::Vector2D>(json["velocity"]);
			delta_time_ = game->get_component<Component::ValTemplate<float>>("delta_time");
		}

		void execute() override
		{
			auto dir = (*direction_);

			if (direction_->y != 0.0f && direction_->x != 0.0f)
				dir *= ONE_OVER_SQRT_TWO;
			
			velocity_->x += dir.x * speed_->val;
			velocity_->y += dir.y * speed_->val;
		}

		std::string get_id() override { return "system-move"; }
	};
}
