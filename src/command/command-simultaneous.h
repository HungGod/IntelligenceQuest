#pragma once
#include "ec/component/factory/component-factory-command_map.h"
#include "ec/component/component-pathway.h"

namespace Command
{
	class Simultaneous : public ICommand
	{
		std::vector<ICommand*> commands_;
		bool first_;
	public:

		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
			commands_.clear();

			auto command_map = game->get_component<Factory::CommandMap>("command_map");

			for (auto js : json)
				for (auto& obj : js.get<nlohmann::json::object_t>())
					commands_.push_back(pathway->get_command(obj, game, command_map));
			first_ = true;
		}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
			if (first_)
			{
				for (auto comm : commands_)
					pathway->next(comm);
				first_ = false;
			}

			bool done = true;
			for (auto comm : commands_)
				done *= !comm->is_in_use();
			
			if (!done)
				pathway->next(this);
		}
		std::string to_string() override { return "simultaneous"; }
	};
}