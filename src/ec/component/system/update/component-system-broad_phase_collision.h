#pragma once
#include "ec/component/system/component-system.h"
#include "ec/component/component-quadtree.h"
#include "ec/component/component-template.h"
#include "ec/entity.h"
#include "ec/component/component-vec2.h"
#include "ec/component/component-pathway.h"
#include "ec/component/component-collider.h"
#include <vector>

namespace System
{
	class BroadPhaseCollision : public ISystem
	{
		Component::Template<std::vector<Component::Collider*>>* moveable_colliders_{};
		Component::Template<std::vector<Component::Collider*>>* static_colliders_{};
		Component::QuadTree* tree_{};
		Component::Pathway* pathway_{};
		Component::ColliderMask* mask_{};
		Component::ValTemplate<float>* delta_time_{};
	public:

		void init(nlohmann::json json, Entity* game) override
		{
			moveable_colliders_ = game->get_child("Collision")->get_component<Component::Template<std::vector<Component::Collider*>>>("moveable_colliders");
			static_colliders_ = game->get_child("Collision")->get_component<Component::Template<std::vector<Component::Collider*>>>("static_colliders");
			tree_ = game->get_nested_component<Component::QuadTree>(json["tree"]);

			mask_ = game->get_child("Collision")->get_component<Component::ColliderMask>("mask");
			pathway_ = game->get_component<Component::Pathway>("pathway");
			delta_time_ = game->get_component<Component::ValTemplate<float>>("delta_time");
		}

		void execute() override
		{
			tree_->all_clear();
			tree_->add(*moveable_colliders_);	
			tree_->add(*static_colliders_);

			for (Component::Collider* col_a : *moveable_colliders_){
				col_a->position->x += col_a->velocity->x * delta_time_->val;
				col_a->position->y += col_a->velocity->y * delta_time_->val;

				for (Component::Collider* col_b : tree_->retrieve(col_a)){
					col_a->collide_and_resolve(col_b, mask_, pathway_, delta_time_);
				}
				col_a->velocity->x = 0.0f;
				col_a->velocity->y = 0.0f;
			}
		}
			
		std::string get_id() override { return "system-broad_phase_collision"; }
	};
}