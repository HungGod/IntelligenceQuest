#pragma once
#include "command/command.h"
#include "ec/component/controller/component-controller.h"
#include "ec/component/component-pathway.h"
#include "json_to.h"

namespace Command
{
	class ControllerNavigate : public ICommand
	{
		IController* controller_;
		std::vector<nlohmann::json> messages_;
		int pos;
	public:
		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
			controller_ = game->get_nested_component<IController>(json["controller"]);
			JsonTo json_to;

			for (auto js : json_to.vector(json["commands"]))
				messages_.push_back(js);
			pos = 0;
			pathway->message(messages_.front());
		}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
			if (controller_->key_press_action_1())
			{
				if (++pos < messages_.size())
					pathway->message(messages_[pos]);
				else
					return;
			}

			if (controller_->key_press_action_2())
			{
				if (--pos >= 0)
					pathway->message(messages_[pos]);
				else
					pos = 0;
			}
		}
		std::string to_string() override { return "navigate_textbox"; }
	};
}