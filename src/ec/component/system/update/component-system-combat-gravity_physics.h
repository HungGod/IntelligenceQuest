#pragma once
#include "ec/component/component-vec2.h"
#include "ec/component/component-template.h"
#include "ec/component/system/component-system.h"

/*
namespace Component
{
	class Velocities : public std::vector<glm::vec2>{};
}
*/

namespace System
{
	class GravityPhysics : public ISystem
	{
		Component::Position* camera_position_;
		Component::Float* camera_zoom_;
		Component::Float* screen_h_;
		Component::Float* delta_time_;
		float gravity_;
		Component::Position* position_;
		Component::Float* scale_;
		Component::Float* height_;
		Component::Velocity* velocity_;
		Component::Bool* midair_;

	public:

		void init(nlohmann::json json, Entity* game) override
		{
			
			position_ = game->get_nested_component<Component::Position>(json["position"]),
			scale_ = game->get_nested_component<Component::Float>(json["scale"]),
			height_ = game->get_nested_component<Component::Float>(json["height"]),
			velocity_ = game->get_nested_component<Component::Velocity>(json["velocity"]),
			midair_ = game->get_nested_component<Component::Bool>(json["midair"]);

			camera_position_ = game->get_nested_component<Component::Position>(json["camera_position"]);
			camera_zoom_ = game->get_nested_component<Component::Float>(json["camera_zoom"]);
			gravity_ = json["gravity"];
			screen_h_ = game->get_component<Component::Float>("height");
			delta_time_ = game->get_component<Component::Float>("delta_time");
		}

		void execute() override
		{
			const float dt = delta_time_->val;
			/* 2) integrate gravity & clamp ---------------------------------------- */
			// Calculate feet position (bottom of fighter) - before physics update
			float feet = position_->y + height_->val * scale_->val;
			// Calculate ground level (bottom of screen in world coordinates)
			float ground = camera_position_->y + screen_h_->val / camera_zoom_->val;

			// If fighter is above ground, apply downward push force incrementally
			if (feet < ground) {
				// Apply downward acceleration proportional to gravity factor
				velocity_->y += gravity_;
				midair_->val = true;
			}
			// Update position based on velocity

			position_->x += velocity_->x * dt;
			position_->y += velocity_->y * dt;

			velocity_->x = 0.0f;
			velocity_->y = 0.0f;

			// Recalculate feet position after physics update
			feet = position_->y + height_->val * scale_->val;

			// If fighter goes beneath ground, clamp back to ground
			if (feet > ground) {
				midair_->val = false;
				position_->y = ground - height_->val * scale_->val;
				velocity_->y = 0.f;
			}
		}
		std::string get_id() override { return "system-combat-physics"; }
	};
}