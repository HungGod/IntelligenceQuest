#pragma once
#include "command/command.h"

namespace Command
{
	class PrintGamestate : public ICommand
	{
	public:

		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
			game->print();
		}
		std::string to_string() override { return "print_game_state"; }
	};
}

