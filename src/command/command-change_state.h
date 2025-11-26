#pragma once
#include "command/command.h"
#include "templates.h"
#include "ec/component/component-template.h"
#include "ec/component/system/component-system.h"
#include "json_to.h"
#include "ec/entity.h"

namespace Command
{
	class ChangeState : public ICommand
	{
		std::vector<nlohmann::json> render_;
		std::vector<nlohmann::json> update_;
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
		}
		std::string to_string() override { return "change_state"; }
		
	};
}