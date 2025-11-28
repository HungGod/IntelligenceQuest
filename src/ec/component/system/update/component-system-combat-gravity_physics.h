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
		struct Fighter {
			Component::Position* position;
			Component::Float* scale;
			Component::Float* height;
			Component::Velocity* vel;
			Component::Bool* midair;
		};

		std::vector<Fighter> fighters_;
	public:

		void init(nlohmann::json json, Entity* game) override
		{
			for (auto fighter_j : json["fighters"])
			{
				fighters_.push_back({
					game->get_nested_component<Component::Position>(fighter_j["position"]),
					game->get_nested_component<Component::Float>(fighter_j["scale"]),
					game->get_nested_component<Component::Float>(fighter_j["height"]),
					game->get_nested_component<Component::Velocity>(fighter_j["velocity"]),
					game->get_nested_component<Component::Bool>(fighter_j["midair"])
					});
			}

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
			for (auto& f : fighters_) {
				// Calculate feet position (bottom of fighter) - before physics update
				float feet = f.position->y + f.height->val * f.scale->val;
				// Calculate ground level (bottom of screen in world coordinates)
				float ground = camera_position_->y + screen_h_->val / camera_zoom_->val;

				// If fighter is above ground, apply downward push force incrementally
				if (feet < ground) {
					// Apply downward acceleration proportional to gravity factor
					f.vel->y += gravity_;
					f.midair->val = true;
				}
				// Update position based on velocity

				f.position->x += f.vel->x * dt;
				f.position->y += f.vel->y * dt;

				f.vel->x = 0.0f;
				f.vel->y = 0.0f;

				// Recalculate feet position after physics update
				feet = f.position->y + f.height->val * f.scale->val;

				// If fighter goes beneath ground, clamp back to ground
				if (feet > ground) {
					f.midair->val = false;
					f.position->y = ground - f.height->val * f.scale->val;
					f.vel->y = 0.f;
				}
			}

		}
		std::string get_id() override { return "system-combat-physics"; }
	};
}