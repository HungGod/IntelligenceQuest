#pragma once
#include "ec/component/system/component-system.h"
#include "ec/component/component-quadtree.h"

namespace System
{
	class ClearQuadTree : public ISystem
	{
		Component::QuadTree* quadtree_;
	public:
		void init(nlohmann::json json, Entity* game)
		{
			if (json.contains("quadtree"))
				quadtree_ = game->get_nested_component<Component::QuadTree>(json["quadtree"]);
			else if (!json.is_null())
				quadtree_ = game->get_nested_component<Component::QuadTree>(json);
		}

		void execute() override
		{
			quadtree_->all_clear();
		}

		std::string get_id() override { return "system-clear_quadtree"; }
	};
}