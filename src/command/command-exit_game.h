#pragma once
#include "command/command.h"
#include "ec/component/component-template.h"


namespace Command
{
	class ExitGame : public ICommand
	{
	public:

		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
			Component::ValTemplate<bool>* exit = game->get_component<Component::ValTemplate<bool>>("exit");
			exit->val = true;
		}
		std::string to_string() override { return "add_flag"; }
	};
}