#pragma once
#include "command/command.h"
#include "ec/entity.h"

namespace Command
{
	class UpdateComponents : public ICommand
	{
		nlohmann::json path_;
		nlohmann::json update_;
	public:
		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
			path_ = json["path"];
			update_ = json["update"];
		}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
			for (auto c : game->get_nested_child(path_)->component_list())
				c->init(update_, game);
		}
		std::string to_string() override { return "update_components"; }
	};
}