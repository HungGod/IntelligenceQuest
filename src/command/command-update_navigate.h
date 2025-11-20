#pragma once
#include "command/command.h"
#include "ec/component/system-navigate/component-system-navigate.h"


namespace Command
{
	class UpdateNavigate : public ICommand
	{
		nlohmann::json json_;
	public:

		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
			json_ = json;
		}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
			ANavigate* navigate = game->get_nested_component<ANavigate>(json_["navigate"]);
			navigate->load_new_commands(json_["commands"]);
		}

		std::string to_string() override { return "add_flag"; }
	};
}