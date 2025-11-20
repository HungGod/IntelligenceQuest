#pragma once
#include "ec/component/src/component-src.h"
#include "ec/component/component-animation_map.h"
#include "ec/entity.h"
namespace Component
{
	class Animation : public IComponent
	{
	public:
		void init(nlohmann::json json, Entity* game)
		{
			if (json.contains("animation_map"))
			{
				auto& animation_map = *game->get_nested_component<Component::AnimationMap>(json["animation_map"]);
				auto id = json["id"].get<std::string>();
				val = &animation_map[id];
			}
			else
				val = nullptr;
		}

		std::vector<ISrc*>* val;

		std::string get_id() override { return "animation"; }
	};
}