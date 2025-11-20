#pragma once
#include "ec/component/system-navigate/component-system-navigate.h"
#include "ec/component/controller/component-controller.h"
#include "ec/component/component-pathway.h"
#include "ec/component/block-options/component-block-options.h"


namespace Navigate
{
	class Horizontal : public ANavigate
	{
		nlohmann::json commands_;
		Component::Pathway* pathway_;
		IController* controller_;
		float real_pos_;
		float step_;
		Component::Int* nav_pos_;
	public:

		void init(nlohmann::json json, Entity* game) override final
		{
			pathway_ = game->get_component<Component::Pathway>("pathway");
			controller_ = game->get_nested_component<IController>(json["controller"]);
			nav_pos_ = game->get_nested_component<Component::Int>(json["nav_pos"]);
			step_ = json["step"].get<float>();

			real_pos_ = 0.0f;
		}

		void load_new_commands(nlohmann::json n_commands) override
		{
			nav_pos_->val = 0;
			commands_ = n_commands;
		}

		void execute() override
		{
			if (controller_->key_down_left())
				real_pos_ -= step_;
			if (controller_->key_down_right())
				real_pos_ += step_;

			nav_pos_->val = static_cast<int>(real_pos_);

			if (nav_pos_->val >= commands_.size())
			{
				real_pos_ = 0;
				nav_pos_->val = 0;
			}
			
			if (nav_pos_->val < 0)
			{
				real_pos_ = static_cast<float>(commands_.size() - 1);
				nav_pos_->val = static_cast<float>(commands_.size() - 1);
			}

			if (controller_->key_press_action_1())
			{
				pathway_->message(commands_[nav_pos_->val]);
			}

		}

		std::string get_id() override { return "system-navigate-horizontal"; }
	};
}