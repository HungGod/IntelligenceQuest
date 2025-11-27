#pragma once
#include "command/command.h"
#include "ec/component/component-template.h"
#include "templates.h"
#include "ec/component/system/component-system.h"
#include "ec/component/component-cache.h"
#include "ec/component/component-collider.h"
#include "ec/component/component-quadtree.h"

namespace Command
{
	class Pause : public ICommand
	{
		nlohmann::json json_;
	public:

		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
			json_ = json;
		}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
			Component::SystemVector& game_render = *game->get_component<Component::SystemVector>("render");
			Component::SystemVector& game_update = *game->get_component<Component::SystemVector>("update");
			
			if (json_.contains("cache"))
				if (json_["cache"] == true)
				{
					Component::SystemCache* render_cache = game->get_child("Cache")->get_component<Component::SystemCache>("render");
					Component::SystemCache* update_cache = game->get_child("Cache")->get_component<Component::SystemCache>("update");

					render_cache->add_to_cache(game_render);
					update_cache->add_to_cache(game_update);
				}

			if (json_.contains("pause_render"))
				if (json_["pause_render"] == true)
					game_render.clear();
			
			if (json_.contains("pause_update"))
				if (json_["pause_update"] == true)
					game_update.clear();

			if (json_.contains("render"))
			{
				std::vector<ISystem*> systems;

				for (auto& v : json_["render"])
				{
					auto child_systems = game->get_nested_child(v)->get_child("Render")->casted_component_list<ISystem>();
					systems.insert(systems.begin(), child_systems.begin(), child_systems.end());
				}

				if (json_.contains("render_insert"))
				{
					int pos = json_["render_insert"];
					game_render.insert(game_render.begin() + pos, systems.begin(), systems.end());
				}
				else
				{
					if (game_render.empty())
						game_render.insert(game_render.begin(), systems.begin(), systems.end());
					else
						game_render.insert(game_render.end() - 1, systems.begin(), systems.end());
				}
			}

			if (json_.contains("update"))
			{
				std::vector<ISystem*> systems;
				for (auto& v : json_["update"])
				{
					auto child_systems = game->get_nested_child(v)->get_child("Update")->casted_component_list<ISystem>();
					systems.insert(systems.begin(), child_systems.begin(), child_systems.end());
				}

				if (json_.contains("update_insert"))
				{
					int pos = json_["update_insert"];
					game_update.insert(game_update.begin() + pos, systems.begin(), systems.end());
				}
				else
				{
					if (game_update.empty())
						game_update.insert(game_update.begin(), systems.begin(), systems.end());
					else
						game_update.insert(game_update.end() - 1, systems.begin(), systems.end());
				}

			}

			if (json_.contains("clear_colliders"))
			{
				Component::ColliderVector* moveable_colliders = game->get_child("Collision")->get_component<Component::ColliderVector>("moveable_colliders");
				Component::ColliderVector* static_colliders = game->get_child("Collision")->get_component<Component::ColliderVector>("static_colliders");
				Component::QuadTree* quadtree = game->get_child("Collision")->get_child("QuadTree")->get_component<Component::QuadTree>("root");

				moveable_colliders->clear();
				static_colliders->clear();
				quadtree->all_clear();
			}

		}

		std::string to_string() override { return "pause"; }
	};
}