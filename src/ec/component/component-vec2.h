#pragma once
#include "ec/component/component.h"
#include <glm/glm.hpp>
#include "ec/component/component-position_map.h"
#include "ec/component/component-template.h"
#include "templates.h"
#include "ec/component/src/component-src.h"
#include "ec/component/block/component-block-box.h"

namespace Component
{
	class Vector2D : public IComponent, public glm::vec2
#define Position Vector2D
#define Direction Vector2D
	{
	public:
		Vector2D()
		{}

		void init(nlohmann::json json, Entity* game) override
		{
			if (json.contains("position_map"))
			{
				Component::PositionMap* position_map = game->get_nested_component<Component::PositionMap>(json["position_map"]);
				std::size_t id = json["id"];
				int num = 0;
				if (json.contains("num"))
					num = json["num"];
				Component::Float* scale = game->get_nested_component<Component::Float>(json["scale"]);
				ISrc* src_temp = game->get_nested_component<ISrc>(json["src_temp"]);

				glm::vec2 pos = position_map->at(id)[num];
				this->x = pos.x;
				this->y = pos.y;
			}
			else if (json.contains("obj") && json.contains("box"))
			{
				Component::Position* obj = game->get_nested_component<Component::Vector2D>(json["obj"]);
				Component::Float* obj_width = game->get_nested_component<Component::Float>(json["obj_width"]);
				Component::Float* scale = game->get_nested_component<Component::Float>(json["obj_scale"]);
				Block::Box* box = game->get_nested_component<Block::Box>(json["box"]);

				this->x = obj->x - box->get_arrow_position().x + obj_width->val * scale->val / 2.0f;
				this->y = obj->y - box->get_arrow_position().y;
				
			}
			else if (json.contains("x"))
			{
				this->x = json["x"];
				this->y = json["y"];
			}
			else if (json.contains("displace"))
			{
				this->x += json["displace"]["x"].get<float>();
				this->y += json["displace"]["y"].get<float>();
			}
			else
			{
				this->x = 0;
				this->y = 0;
			}
		}
		std::string get_id() override { return "vec2"; }
	};
}