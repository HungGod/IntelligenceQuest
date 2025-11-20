#pragma once
#include "ec/component/system/component-system.h"
#include "ec/component/component-quadtree.h"
#include "ec/component/component-template.h"
#include "templates.h"
#include "ec/entity.h"
#include "ec/component/component-vec2.h"
#include "ec/component/component-pathway.h"
#include "ec/component/collider/component-collider.h"

namespace System
{
	class BroadPhaseCollision : public ISystem
	{
		Component::ColliderVector* moveable_colliders_{};
		Component::QuadTree* tree_{};
		Component::Pathway* pathway_{};
		Component::ColliderMask* mask_{};
	public:

		void init(nlohmann::json json, Entity* game) override
		{
			moveable_colliders_ = game->get_nested_component<Component::ColliderVector>(json["moveable_colliders"]);

			tree_ = game->get_nested_component<Component::QuadTree>(json["quadtree"]);

			mask_ = game->get_child("Colliders")->get_component<Component::ColliderMask>("mask");
			pathway_ = game->get_component<Component::Pathway>("pathway");
		}

		void execute() override
		{
			for (Component::Collider* col_a : *moveable_colliders_)
				for (Component::Collider* col_b : tree_->retrieve(col_a))
					col_a->collide_and_resolve(col_b, mask_, pathway_);
		}
			
		std::string get_id() override { return "system-broad_phase_collision"; }
	};
}