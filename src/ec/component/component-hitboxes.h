#pragma once
#include "ec/component/gjk/component-gjk.h"
#include "ec/entity.h"

namespace Component
{
	class Hitboxes : public IComponent, public std::vector<Component::Collider*>
	{
	public:
		void init(nlohmann::json json, Entity* game)
		{
			if (!json.is_null())
			{
				std::vector<Component::Collider*> colliders = game->get_nested_child(json)->casted_component_list<Component::Collider>();
				this->insert(this->end(), colliders.begin(), colliders.end());
			}
		}
		std::string get_id() override { return "hitboxes"; }
	};
}