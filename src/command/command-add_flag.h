#pragma once
#include "command/command.h"
#include "templates.h"
#include "ec/component/component-template.h"


namespace Command
{
	class AddFlag : public ICommand
	{
		nlohmann::json json_;
	public:

		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
			json_ = json;
		}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
			auto flags = game->get_nested_component<Component::Json>(json_["flags"]);
			flags->val[json_["flag_id"].get<std::string>()] = true;
		}
		std::string to_string() override { return "add_flag"; }
	};
}