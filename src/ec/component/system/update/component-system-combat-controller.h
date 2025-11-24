#pragma once
#include "ec/component/system/component-system.h"
#include "ec/component/controller/component-controller.h"
#include "ec/component/component-vec2.h"
#include "ec/component/component-pathway.h"
#include "ec/component/component-template.h"

namespace System
{
	class CombatController : public ISystem
	{
		IController* controller_{};
		Component::Direction* direction_{};
		Component::Json* ground_ability_1{}, * ground_ability_2{}, * ground_ability_3{},
			* air_ability_1{}, * air_ability_2{}, * air_ability_3{}, * jump{};
		Component::Bool* midair{};
		Component::Pathway* pathway_{};
	public:
		CombatController() = default;

		void init(nlohmann::json json, Entity* game) override
		{
			controller_ = game->get_nested_component<IController>(json["controller"]);
			direction_ = game->get_nested_component<Component::Direction>(json["direction"]);
		}

		void execute() override
		{
			// get the current state of movement for our character
			direction_->x = (float)controller_->key_down_right() - (float)controller_->key_down_left();

			if (controller_->key_down_up())
				if (!midair->val)
					pathway_->message(jump->val);
			

			if (controller_->key_press_action_1())
			{
				if (midair->val)
					pathway_->message(air_ability_1->val);
				else
					pathway_->message(ground_ability_1->val);
			}

			if (controller_->key_press_action_2())
			{
				if (midair->val)
					pathway_->message(air_ability_2->val);
				else
					pathway_->message(ground_ability_2->val);
			}

			if (controller_->key_press_action_3())
			{
				if (midair->val)
					pathway_->message(air_ability_3->val);
				else
					pathway_->message(ground_ability_3->val);
			}
		}
		std::string get_id() override { return "system-combat-controller"; }
	};
}