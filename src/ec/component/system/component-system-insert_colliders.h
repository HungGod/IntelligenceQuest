#pragma once
#include "ec/component/gjk/component-gjk.h"
#include "ec/component/component-quadtree.h"
#include "ec/component/system/component-system.h"
#include "templates.h"
#include "ec/component/component-tilemap.h"
#include "ec/component/component-vec2.h"
#include "ec/component/collider/component-collider.h"

namespace System
{
	class InsertColliders : public ISystem 
	{
		std::vector<Component::Collider*> colliders_;
		Component::QuadTree* quadtree_;
	public:

		InsertColliders()
			: colliders_(), quadtree_(nullptr)
		{}

		void init(nlohmann::json json, Entity* game) override
		{
			colliders_ = game->get_nested_child(json["colliders"])->casted_component_list<Component::Collider>();
			quadtree_ = game->get_nested_component<Component::QuadTree>(json["quadtree"]);
		}

		void execute() override
		{
			for (auto col : colliders_)
				quadtree_->add(col);
		}

		std::string get_id() override { return "system-insert_tilemap_colliders"; }
	};
}