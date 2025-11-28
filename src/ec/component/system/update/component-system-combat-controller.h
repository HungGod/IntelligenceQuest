#pragma once
#include "ec/component/system/component-system.h"
#include "ec/component/controller/component-controller.h"
#include "ec/component/component-vec2.h"
#include "ec/component/component-pathway.h"
#include "ec/component/component-template.h"
#include "templates.h"

namespace System
{
	class CombatController : public ISystem
	{
		IController* controller_{};
		Component::Direction* direction_{};
		Component::Json* combat_abilities{};
		Component::Bool* midair_{};
		Component::Pathway* pathway_{};
		Component::Float* speed_{};
		Component::Velocity* velocity_{};
	public:
		CombatController() = default;

		void init(nlohmann::json json, Entity* game) override
		{
			midair_ = game->get_nested_component<Component::Bool>(json["midair"]);
			combat_abilities = game->get_nested_component<Component::Json>(json["combat_abilities"]);
			controller_ = game->get_nested_component<IController>(json["controller"]);
			direction_ = game->get_nested_component<Component::Direction>(json["direction"]);
			speed_ = game->get_nested_component<Component::Float>(json["speed"]);
			velocity_ = game->get_nested_component<Component::Velocity>(json["velocity"]);
			pathway_ = game->get_component<Component::Pathway>("pathway");
		}

		void execute() override
		{
			// get the current state of movement for our character
			direction_->x = (float)controller_->key_down_right() - (float)controller_->key_down_left();
			velocity_->x += direction_->x * speed_->val;
			if (controller_->key_down_up())
				if (!midair_->val)
				{
					midair_->val = true;
					pathway_->message(combat_abilities->val["jump"]);
				}

			if (controller_->key_press_action_1())
			{
				if (midair_->val)
					pathway_->message(combat_abilities->val["air_ability_1"]);
				else
					pathway_->message(combat_abilities->val["ground_ability_1"]);
			}

			if (controller_->key_press_action_2())
			{
				if (!midair_->val)
					pathway_->message(combat_abilities->val["ground_ability_2"]);
			}

			if (controller_->key_press_action_3())
			{
				if (!midair_->val)
					pathway_->message(combat_abilities->val["ground_ability_3"]);
			}
		}
		std::string get_id() override { return "system-combat-controller"; }
	};
}