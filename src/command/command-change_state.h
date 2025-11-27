#pragma once
#include "command/command.h"
#include "templates.h"
#include "ec/component/component-template.h"
#include "ec/component/system/component-system.h"
#include "json_to.h"
#include "ec/entity.h"
#include "ec/component/component-collider.h"

namespace Command
{
	class ChangeState : public ICommand
	{
		std::vector<nlohmann::json> render_;
		std::vector<nlohmann::json> update_;
		bool add_colliders_to_quadtree_{};
	public:
		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
			JsonTo json_to;
			if (json.contains("render"))
				render_ = json_to.vector(json["render"]);
			else
				render_.clear();

			if (json.contains("update"))
				update_ = json_to.vector(json["update"]);
			else
				update_.clear();

			add_colliders_to_quadtree_ = json.value("add_colliders_to_quadtree", false);
		}

		void execute(Entity* game, Component::Pathway* pathway) override
		{

			auto set_system_vec = [game] (std::vector<nlohmann::json> vec, std::string sys_name, std::string child_name)
			{
				auto& c_system = *game->get_component<Component::SystemVector>(sys_name);

				std::vector<ISystem*> systems;
				for (auto &v : vec)
				{
					Entity* child = game->get_nested_child(v);
					if (child->has_child(child_name))
					{
						auto child_systems = child->get_child(child_name)->casted_component_list<ISystem>();
						systems.insert(systems.begin(), child_systems.begin(), child_systems.end());
					}
				}

				c_system.clear();
				c_system.insert(c_system.begin(), systems.begin(), systems.end());
			};

			set_system_vec(render_, "render", "Render");
			set_system_vec(update_, "update", "Update");

			if (add_colliders_to_quadtree_)
			{
				Component::ColliderVector* moveable_colliders = game->get_child("Collision")->get_component<Component::ColliderVector>("moveable_colliders");
				Component::ColliderVector* static_colliders = game->get_child("Collision")->get_component<Component::ColliderVector>("static_colliders");
				moveable_colliders->clear();
				static_colliders->clear();
				for (auto& rj : render_)
				{
					Entity* e = game->get_nested_child(rj);

					if (e->has_child("Colliders"))
					{
						Entity* e_colliders = e->get_child("Colliders");
						
						for (auto& c : e_colliders->casted_component_list<Component::Collider>())
						{
							if (c->add_collider)
							{
								if (c->moveable)
									moveable_colliders->push_back(c);
								else
									static_colliders->push_back(c);
							}
						}
					}
					
				}
			}
		}
		std::string to_string() override { return "change_state"; }
		
	};
}