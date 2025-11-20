#pragma once
#include "command/command.h"
#include "ec/component/component-intelligence.h"
#include "ec/component/component-pathway.h"

namespace Command
{
	class SmartEnough : public ICommand
	{
		int iq;
		nlohmann::json commands;

	public:
		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
			iq = json["IQ"];
			commands = json["commands"];
		}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
			auto player_int = game->get_child("Player")->get_component<Component::Intelligence>("intelligence");

			if (player_int->total >= iq)
				pathway->message(commands);
		}
		std::string to_string() override { return "smart_enough"; }
	};
}