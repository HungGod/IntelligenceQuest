#pragma once
#include "ec/component/component-intelligence.h"
#include "command/command.h"
#include "ec/component/system/render/component-system-text_draw.h"

namespace Command
{
	class AddIntelligence : public ICommand
	{
		nlohmann::json json_;

	public:
		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
			json_ = json;
		}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
			auto intelligence = game->get_child("Player")->get_component<Component::Intelligence>("intelligence");
			if (!json_.is_null())
			{
				intelligence->add_intelligence(json_);

				std::cout << "IQ: " << intelligence->total << std::endl;
			}
		}
		std::string to_string() override { return "add_intelligence"; }
	};
}