#pragma once
#include "command/command.h"
#include "ec/component/block-options/component-block-options.h"


namespace Command
{
	class UpdateOptions : public ICommand
	{
		nlohmann::json json_;
	public:

		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
			json_ = json;
		}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
			auto options = game->get_nested_component<AOptions>(json_["options_path"]);
			options->change_options(json_["options"]);
		}

		std::string to_string() override { return "update_options"; }
	};
}