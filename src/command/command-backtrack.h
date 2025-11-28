#pragma once
#include "command/command.h"
#include "ec/component/component-template.h"
#include "ec/component/system/component-system.h"
#include "ec/component/component-cache.h"

namespace Command
{
	class Backtrack : public ICommand
	{
	public:

		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
		}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
			Component::SystemCache* render_cache = game->get_child("Cache")->get_component<Component::SystemCache>("render");
			Component::SystemCache* update_cache = game->get_child("Cache")->get_component<Component::SystemCache>("update");

			Component::Template<std::vector<ISystem*>>* game_render = game->get_component<Component::Template<std::vector<ISystem*>>>("render");
			Component::Template<std::vector<ISystem*>>* game_update = game->get_component<Component::Template<std::vector<ISystem*>>>("update");

			std::vector<ISystem*> previous_render = render_cache->get_previous_cache();
			std::vector<ISystem*> previous_update = update_cache->get_previous_cache();

			game_render->clear();
			game_render->insert(game_render->end(), previous_render.begin(), previous_render.end());

			game_update->clear();
			game_update->insert(game_update->end(), previous_update.begin(), previous_update.end());
		}

		std::string to_string() override { return "backtrack"; }
	};
}