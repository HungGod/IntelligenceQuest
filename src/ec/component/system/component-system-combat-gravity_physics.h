#pragma once
#include "ec/component/component-vec2.h"
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
		float gravity_;
		struct Fighter {
			Component::Position* position;
			Component::Float* scale;
			Component::Float* height;
			Component::Vector2D* vel;
			Component::Vector2D* impulse;
		};

		std::vector<Fighter> fighters;
	public:

		void init(nlohmann::json json, Entity* game) override
		{
			for (auto fighter_j : json["fighters"])
			{
				fighters.push_back({
					game->get_nested_component<Component::Position>(fighter_j["position"]),
					game->get_nested_component<Component::Float>(fighter_j["scale"]),
					game->get_nested_component<Component::Float>(fighter_j["height"]),
					game->get_nested_component<Component::Vector2D>(fighter_j["velocity"])
					});
			}

			camera_position_ = game->get_nested_component<Component::Position>(json["camera_position"]);
			camera_zoom_ = game->get_nested_component<Component::Float>(json["camera_zoom"]);
			gravity_ = json["gravity"];
			screen_h_ = game->get_component<Component::Float>("height");
		}

		void execute() override
		{
			/* 1) accumulate impulses → velocity ----------------------------------- */
			for (auto f : fighters) {
				f.vel->v += f.impulse->i;
				f.impulse->i = {};                      // clear events
			}

			/* 2) integrate gravity & clamp ---------------------------------------- */
			for (auto f : fighters) {
				f.vel->v.y += gravity * dt;
				f.pos->p += f.vel->v * dt;

				float feet = f.pos->p.y + f.size->h * f.scale->s;
				float ground = cam_pos->y + screen_h / cam_zoom;

				if (feet > ground) {
					f.pos->p.y = ground - f.size->h * f.scale->s;
					f.vel->v.y = 0.f;
				}
			}

		}
		std::string get_id() override { return "system-combat-physics"; }
	};
}