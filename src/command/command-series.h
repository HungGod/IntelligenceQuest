#pragma once
#include "ec/component/factory/component-factory-command_map.h"
#include "ec/component/component-pathway.h"

namespace Command
{
	class Series : public ICommand
	{
		std::vector<ICommand*> commands_;
		int pos_;
	public:

		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
			commands_.clear();
			pos_ = 0;

			auto command_map = game->get_component<Factory::CommandMap>("command_map");

			for (auto js : json)
			{
				if (js.is_array())
					Logger::error("Series trying to run concurrently, when it shouldn't be", Logger::HIGH);

				for (auto& obj : js.get<nlohmann::json::object_t>())
					commands_.push_back(pathway->get_command(obj, game, command_map));

			}
				
			pathway->next(commands_[pos_]);
		}

		void execute(Entity* game, Component::Pathway* pathway) override
		{

			if (!commands_[pos_]->is_in_use())
			{
				if (++pos_ < commands_.size())
					pathway->next(commands_[pos_]);
				else
					return;
			}

			pathway->next(this);
		}
		std::string to_string() override { return "series"; }
	};
}