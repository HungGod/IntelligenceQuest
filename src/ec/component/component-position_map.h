#pragma once
#include "ec/component/component.h"
#include <unordered_map>
#include <glm/glm.hpp>
#include "ec/component/component-tilemap.h"
#include "ec/component/component-template.h"
#include "ec/entity.h"

namespace Component
{
	class PositionMap : public IComponent, public std::unordered_map<std::size_t, std::vector<glm::vec2>>
	{
	public:
		void init(nlohmann::json json, Entity* game) override
		{
			auto tilemap = game->get_nested_component<Component::Tilemap>(json["tilemap"]);
			auto width = tilemap->width;
			auto tile_size = tilemap->tile_size;
			auto scale = game->get_nested_component<Component::ValTemplate<float>>(json["scale"]);
			this->clear();
			auto i = 0;
			for (int pos : json["map"])
			{
				if (pos)
				{
					auto temp_pos = glm::vec2((i % (int)width) * tile_size * scale->val, (i / (int)width) * tile_size * scale->val - 128);
					(*this)[pos - 1].push_back(temp_pos);
				}
					
				i++;
			}
		}
		std::string get_id() override { return "position_map"; }
	};
}