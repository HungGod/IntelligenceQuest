#pragma once
#include "command/command.h"
#include "json_to.h"
#include "ec/entity.h"


namespace Command
{
	class CreateComponents : public ICommand
	{
		nlohmann::json json_;
	public:

		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
			json_ = json;
		}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
			JsonTo json_to;
			auto temp = json_;
			auto e = game;
			if (json_.contains("path"))
			{
				for (auto& p : json_to.vector(json_["path"]))
				{
					if (e->has_child(p.get<std::string>()))
						e = e->get_child(p.get<std::string>());
					else
						e = e->insert_child(p.get<std::string>());
				}
				temp = json_["components"];
			}
			for (auto& j : json_to.vector(temp))
				json_to.component(j, e, game);
		}
		std::string to_string() override { return "create_component"; }
	};
}