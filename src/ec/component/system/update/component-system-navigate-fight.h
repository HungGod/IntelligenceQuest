#pragma once
#pragma once
#include "ec/component/system/update/component-system-navigate.h"
#include "ec/component/controller/component-controller.h"
#include "ec/component/component-pathway.h"

namespace Navigate
{
	class Message : public ANavigate
	{
		nlohmann::json commands_;
		Component::Pathway* pathway_;
		IController* controller_;
		int pos_;
	public:

		void init(nlohmann::json json, Entity* game) override
		{
			pathway_ = game->get_component<Component::Pathway>("pathway");
			controller_ = game->get_nested_component<IController>(json["controller"]);
			pos_ = 0;
		}

		void load_new_commands(nlohmann::json n_commands) override
		{
			pos_ = 0;
			commands_ = n_commands;
			pathway_->message(commands_[pos_]);
		}

		void execute() override
		{
			if (commands_.size() <= 0)
				return;

			if (controller_->key_press_action_1())
			{
				if (++pos_ >= commands_.size())
				{
					Logger::warning("Attempt caught going above command navigation limit", Logger::LOW);
					return;
				}

				pathway_->message(commands_[pos_]);
			}

			else if (controller_->key_press_action_2())
			{
				if (--pos_ <= 0)
					pos_ = 0;

				pathway_->message(commands_[pos_]);
			}
		}


		std::string get_id() override { return "ec/component/component-system-navigate_commands"; }
	};
}