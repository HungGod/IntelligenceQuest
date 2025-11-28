#pragma once
#include "ec/component/system/component-system.h"
#include "ec/component/component-template.h"
#include <GLFW/glfw3.h>
#include "ec/component/component-pathway.h"
#include "ec/entity.h"
#include "ec/component/src/component-src-transform.h"
#include "ec/component/component-animation.h"

namespace System
{
	class Animate : public ISystem
	{
		Component::Animation* animation_;
		Src::Transform* src_temp_;
		Component::ValTemplate<float>* animation_speed_;
	public:
		Animate()
			: animation_speed_(nullptr), animation_(nullptr), src_temp_(nullptr)
		{}

		void init(nlohmann::json json, Entity* game) override
		{
			animation_ = game->get_nested_component<Component::Animation>(json["animation"]);
			src_temp_ = game->get_nested_component<Src::Transform>(json["src_temp"]);
			animation_speed_ = game->get_nested_component<Component::ValTemplate<float>>(json["speed"]);
		}

		void execute() override
		{
			if (animation_->val)
			{
				auto time = static_cast<float>(glfwGetTime());
				auto anim_pos = static_cast<std::size_t>(time * animation_speed_->val) % animation_->val->size(); // find next frame to animate
				src_temp_->src = (*animation_->val)[anim_pos];
			}
		}
		std::string get_id() override { return "system-animate"; }
	};
}