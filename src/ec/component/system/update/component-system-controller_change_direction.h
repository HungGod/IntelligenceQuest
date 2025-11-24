#pragma once
#include "ec/component/system/component-system.h"
#include "ec/component/controller/component-controller.h"
#include "ec/component/component-vec2.h"

namespace System
{
	class ControllerChangeDirection : public ISystem
	{
		IController* controller_;
		Component::Direction* direction_;
	public:
		ControllerChangeDirection()
			: controller_(nullptr), direction_(nullptr)
		{}

		void init(nlohmann::json json, Entity* game) override
		{
			controller_ = game->get_nested_component<IController>(json["controller"]);
			direction_ = game->get_nested_component<Component::Direction>(json["direction"]);
		}

		void execute() override
		{
			// get the current state of movement for our character
			direction_->x = (float)controller_->key_down_right() - (float)controller_->key_down_left(); 
			direction_->y = (float)controller_->key_down_down() - (float)controller_->key_down_up();
		}
		std::string get_id() override { return "system-controller_change_direction"; }
	};
}