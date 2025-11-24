#pragma once
#include "ec/component/system/component-system.h"
#include "ec/component/component-quadtree.h"
#include "ec/component/gjk/component-gjk.h"
#include "ec/component/controller/component-controller.h"
#include "ec/component/component-intelligence.h"
#include "ec/component/component-collider.h"
#include "ec/component/component-collider_mask.h"

namespace System
{
	class CheckAction : public ISystem
	{
		Component::Collider* action_collider_;
		Component::QuadTree* quadtree_;
		IController* controller_;
		Component::Position* position_;
		Component::Pathway* pathway_;
		Component::ColliderMask* mask_;
	public:

		CheckAction()
			: action_collider_(nullptr), quadtree_(nullptr), controller_(nullptr), mask_(nullptr)
		{}

		void init(nlohmann::json json, Entity* game) override
		{
			action_collider_ = game->get_nested_component<Component::Collider>(json["action_collider"]);
			quadtree_ = game->get_nested_component<Component::QuadTree>(json["quadtree"]);
			controller_ = game->get_nested_component<IController>(json["controller"]);
			position_ = game->get_nested_component<Component::Position>(json["position"]);
			pathway_ = game->get_component<Component::Pathway>("pathway");
			mask_ = game->get_child("Colliders")->get_component<Component::ColliderMask>("mask");
		}

		void execute() override
		{
			if (controller_->key_press_action_1())
			{
				std::vector<Component::Collider*> retrieved_cols = quadtree_->retrieve(action_collider_);
				
				for (auto col_b : retrieved_cols)
				{
					if (action_collider_->is_colliding(col_b, mask_))
						pathway_->message(col_b->data["commands"]);
				}
			}
		}

		std::string get_id() override { return "system-check_action"; }
	}; 
}