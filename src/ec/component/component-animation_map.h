#pragma once
#include "ec/component/component.h"
#include "ec/entity.h"

namespace Component
{
	struct AnimationMap : public std::unordered_map<std::string, std::vector<ISrc*>>, public IComponent
	{

		void init(nlohmann::json json, Entity* game)
		{
			auto path = game->get_nested_child(json["path"]);
			auto objects = json["animations"].get<nlohmann::json::object_t>();
			this->clear();
			for (auto obj : objects)
			{
				std::vector<ISrc*> srcs;
				
				if (obj.second.is_array())
					for (auto obj2 : obj.second)
						srcs.push_back(path->get_component<ISrc>(obj2.get<std::string>()));
				else
					srcs.push_back(path->get_component<ISrc>(obj.second.get<std::string>()));

				(*this)[obj.first] = srcs;
			}
		}


		std::string get_id() override { return "animation_map"; }
	};
}