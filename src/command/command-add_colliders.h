#pragma once
#include "command/command.h"
#include "ec/component/component-collider.h"
#include "ec/entity.h"


namespace Command
{
	class AddColliders : public ICommand
	{
		nlohmann::json json_;
	public:

		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
			json_ = json;
		}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
            std::vector<Component::Collider*> e_colliders = game->get_nested_child(json_["colliders"])->casted_component_list<Component::Collider>();
            if (json_["moveable"] == true)
            {
                Component::ColliderVector* moveable_colliders = game->get_child("Collision")->get_component<Component::ColliderVector>("moveable_colliders");
                moveable_colliders->insert(moveable_colliders->end(), e_colliders.begin(), e_colliders.end());
            }
            else
            {
                Component::ColliderVector* static_colliders = game->get_child("Collision")->get_component<Component::ColliderVector>("static_colliders");
                static_colliders->insert(static_colliders->end(), e_colliders.begin(), e_colliders.end());
            }
		}
		std::string to_string() override { return "add_colliders"; }
	};
}