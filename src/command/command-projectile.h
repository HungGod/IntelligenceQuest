#pragma once
#include "command/command.h"
#include "ec/component/component-template.h"
#include "ec/component/component-collider.h"
#include "ec/component/component-vec2.h"
#include "ec/component/src/component-src-transform.h"
#include "ec/component/src/component-src.h"
#include "ec/component/gjk/component-gjk.h"
#include <GLFW/glfw3.h>
#include <cmath>

namespace Command
{
	class Projectile : public ICommand
	{
		// Player spawn point
		Component::Vector2D* player_position_;
		Component::ValTemplate<float>* player_scale_;
		Component::ValTemplate<float>* player_width_;
		Component::Vector2D* direction_;
		
		// Projectile entity references (preloaded)
		Component::Vector2D* projectile_position_;
		Src::Transform* projectile_src_;
		Component::Collider* projectile_collider_;
		
		// Preloaded hitbox shapes
		IGJK* hitbox_left_;
		IGJK* hitbox_right_;
		
		// Preloaded sprites
		ISrc* frame_left_;
		ISrc* frame_right_;
		
		// Properties
		float velocity_;
		float max_distance_;
		float damage_;
		float hitstun_;
		glm::vec2 knockback_;
		glm::vec2 spawn_offset_;
		
		// State
		glm::vec2 start_position_;
		float travel_direction_;
		bool active_;

		void activate_hitbox()
		{
			if (!projectile_collider_) return;
			
			projectile_collider_->shapes.clear();
			
			IGJK* hitbox = (travel_direction_ >= 0) ? hitbox_right_ : hitbox_left_;
			if (hitbox)
				projectile_collider_->shapes.push_back(hitbox);
			
			nlohmann::json hit_data;
			hit_data["damage"] = damage_;
			hit_data["hitstun"] = hitstun_;
			hit_data["knockback"]["x"] = knockback_.x * travel_direction_;
			hit_data["knockback"]["y"] = knockback_.y;
			projectile_collider_->data = hit_data;
		}

		void deactivate()
		{
			if (projectile_collider_)
			{
				projectile_collider_->shapes.clear();
				projectile_collider_->data = nlohmann::json{};
			}
			if (projectile_src_)
				projectile_src_->src = nullptr;
			active_ = false;
		}

		void update_sprite()
		{
			if (!projectile_src_) return;
			projectile_src_->src = (travel_direction_ >= 0) ? frame_right_ : frame_left_;
		}

	public:
		Projectile() = default;

		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
			// Player references
			player_position_ = game->get_nested_component<Component::Vector2D>(json["position"]);
			player_scale_ = game->get_nested_component<Component::ValTemplate<float>>(json["scale"]);
			direction_ = game->get_nested_component<Component::Vector2D>(json["direction"]);
			
			if (json.contains("width"))
				player_width_ = game->get_nested_component<Component::ValTemplate<float>>(json["width"]);
			
			// Projectile entity references
			projectile_position_ = game->get_nested_component<Component::Vector2D>(json["projectile_position"]);
			projectile_src_ = game->get_nested_component<Src::Transform>(json["projectile_src"]);
			projectile_collider_ = game->get_nested_component<Component::Collider>(json["projectile_collider"]);
			
			// Preloaded hitbox shapes
			hitbox_left_ = json.contains("hitbox_left") ?
				game->get_nested_component<IGJK>(json["hitbox_left"]) : nullptr;
			hitbox_right_ = json.contains("hitbox_right") ?
				game->get_nested_component<IGJK>(json["hitbox_right"]) : nullptr;
			
			// Preloaded sprites
			Entity* spritesheet = nullptr;
			if (json.contains("spritesheet"))
				spritesheet = game->get_nested_child(json["spritesheet"]);
			
			frame_left_ = (spritesheet && json.contains("frame_left")) ?
				spritesheet->get_component<ISrc>(json["frame_left"].get<std::string>()) : nullptr;
			frame_right_ = (spritesheet && json.contains("frame_right")) ?
				spritesheet->get_component<ISrc>(json["frame_right"].get<std::string>()) : nullptr;
			
			// Properties
			velocity_ = json.value("projectile_vel", 600.0f);
			max_distance_ = json.value("max_distance", 400.0f);
			damage_ = json.value("damage", 12.0f);
			hitstun_ = json.value("hitstun", 0.25f);
			
			knockback_ = {
				json.contains("knockback") ? json["knockback"].value("x", 180.0f) : 180.0f,
				json.contains("knockback") ? json["knockback"].value("y", -40.0f) : -40.0f
			};
			spawn_offset_ = {
				json.contains("spawn_offset") ? json["spawn_offset"].value("x", 48.0f) : 48.0f,
				json.contains("spawn_offset") ? json["spawn_offset"].value("y", 24.0f) : 24.0f
			};
			
			// Initialize
			travel_direction_ = (direction_->x >= 0) ? 1.0f : -1.0f;
			active_ = true;
			
			// Set spawn position
			if (projectile_position_ && player_position_)
			{
				float width = player_width_ ? player_width_->val : 64.0f;
				projectile_position_->x = player_position_->x + 
					(travel_direction_ >= 0 ? spawn_offset_.x : width - spawn_offset_.x);
				projectile_position_->y = player_position_->y + spawn_offset_.y;
				start_position_ = {projectile_position_->x, projectile_position_->y};
			}
			
			update_sprite();
			activate_hitbox();
		}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
			if (!active_ || !projectile_position_) return;
			
			// Get delta time
			auto delta_time = game->get_component<Component::ValTemplate<float>>("delta_time");
			float dt = delta_time ? delta_time->val : 0.016f;
			
			// Move projectile
			projectile_position_->x += velocity_ * travel_direction_ * dt;
			
			// Check distance
			float distance = std::abs(projectile_position_->x - start_position_.x);
			if (distance >= max_distance_)
			{
				deactivate();
				return;
			}
			
			pathway->next(this);
		}

		std::string to_string() override { return "projectile"; }
	};
}



