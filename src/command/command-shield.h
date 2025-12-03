#pragma once
#include "command/command.h"
#include "ec/component/component-template.h"
#include "ec/component/component-collider.h"
#include "ec/component/component-vec2.h"
#include "ec/component/src/component-src-transform.h"
#include "ec/component/src/component-src.h"
#include "ec/component/gjk/component-gjk.h"
#include <GLFW/glfw3.h>

namespace Command
{
	class Shield : public ICommand
	{
		// Player references
		Component::Vector2D* player_position_;
		Component::ValTemplate<float>* player_scale_;
		Component::ValTemplate<float>* player_width_;
		Component::Vector2D* direction_;
		Component::ValTemplate<bool>* stunned_;
		Component::ValTemplate<bool>* in_shield_;
		
		// Shield entity references (preloaded)
		Component::Vector2D* shield_position_;
		Src::Transform* shield_src_;
		Component::Collider* shield_collider_;
		
		// Preloaded hitbox shapes
		IGJK* hitbox_left_;
		IGJK* hitbox_right_;
		
		// Preloaded sprites
		ISrc* frame_active_;
		ISrc* frame_broken_;
		
		// Shield properties
		float max_health_;
		float current_health_;
		float shrink_rate_;        // Health lost per hit
		float break_stun_;         // Stun duration when shield breaks
		
		// State
		bool shield_broken_;
		double break_time_;
		float facing_direction_;

		void update_position()
		{
			if (!shield_position_ || !player_position_) return;
			shield_position_->x = player_position_->x;
			shield_position_->y = player_position_->y;
		}

		void activate_shield()
		{
			if (!shield_collider_) return;
			
			shield_collider_->shapes.clear();
			
			IGJK* hitbox = (facing_direction_ >= 0) ? hitbox_right_ : hitbox_left_;
			if (hitbox)
				shield_collider_->shapes.push_back(hitbox);
			
			// Shield collider data - marks this as a shield for collision system
			nlohmann::json shield_data;
			shield_data["is_shield"] = true;
			shield_data["absorbs_damage"] = true;
			shield_collider_->data = shield_data;
		}

		void deactivate_shield()
		{
			if (shield_collider_)
			{
				shield_collider_->shapes.clear();
				shield_collider_->data = nlohmann::json{};
			}
		}

		void update_sprite()
		{
			if (!shield_src_) return;
			shield_src_->src = shield_broken_ ? frame_broken_ : frame_active_;
		}

		void break_shield()
		{
			shield_broken_ = true;
			break_time_ = glfwGetTime();
			deactivate_shield();
			
			if (stunned_)
				stunned_->val = true;
			
			update_sprite();
		}

	public:
		Shield() = default;

		// Called externally when shield takes damage
		void take_damage(float damage)
		{
			if (shield_broken_) return;
			
			current_health_ -= damage;
			
			if (current_health_ <= 0)
			{
				current_health_ = 0;
				break_shield();
			}
		}

		float get_health_percent() const
		{
			return current_health_ / max_health_;
		}

		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
			// Player references
			player_position_ = game->get_nested_component<Component::Vector2D>(json["position"]);
			player_scale_ = game->get_nested_component<Component::ValTemplate<float>>(json["scale"]);
			direction_ = game->get_nested_component<Component::Vector2D>(json["direction"]);
			
			if (json.contains("width"))
				player_width_ = game->get_nested_component<Component::ValTemplate<float>>(json["width"]);
			if (json.contains("stunned"))
				stunned_ = game->get_nested_component<Component::ValTemplate<bool>>(json["stunned"]);
			if (json.contains("in_shield"))
				in_shield_ = game->get_nested_component<Component::ValTemplate<bool>>(json["in_shield"]);
			
			// Shield entity references
			shield_position_ = game->get_nested_component<Component::Vector2D>(json["shield_position"]);
			shield_src_ = game->get_nested_component<Src::Transform>(json["shield_src"]);
			shield_collider_ = game->get_nested_component<Component::Collider>(json["shield_collider"]);
			
			// Preloaded hitbox shapes
			hitbox_left_ = json.contains("hitbox_left") ?
				game->get_nested_component<IGJK>(json["hitbox_left"]) : nullptr;
			hitbox_right_ = json.contains("hitbox_right") ?
				game->get_nested_component<IGJK>(json["hitbox_right"]) : nullptr;
			
			// Preloaded sprites
			Entity* spritesheet = nullptr;
			if (json.contains("spritesheet"))
				spritesheet = game->get_nested_child(json["spritesheet"]);
			
			frame_active_ = (spritesheet && json.contains("frame_active")) ?
				spritesheet->get_component<ISrc>(json["frame_active"].get<std::string>()) : nullptr;
			frame_broken_ = (spritesheet && json.contains("frame_broken")) ?
				spritesheet->get_component<ISrc>(json["frame_broken"].get<std::string>()) : nullptr;
			
			// Properties
			max_health_ = json.value("shield_health", 80.0f);
			current_health_ = max_health_;
			shrink_rate_ = json.value("shrink_rate", 10.0f);
			break_stun_ = json.value("break_stun", 1.2f);
			
			// Initialize
			facing_direction_ = (direction_->x >= 0) ? 1.0f : -1.0f;
			shield_broken_ = false;
			
			if (in_shield_)
				in_shield_->val = true;
			
			update_position();
			update_sprite();
			activate_shield();
		}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
			update_position();
			
			if (shield_broken_)
			{
				double elapsed = glfwGetTime() - break_time_;
				
				// End stun after duration
				if (elapsed >= break_stun_)
				{
					if (shield_src_)
						shield_src_->src = nullptr;
					if (stunned_)
						stunned_->val = false;
					if (in_shield_)
						in_shield_->val = false;
					return;
				}
				
				pathway->next(this);
				return;
			}
			
			// Shield still active
			pathway->next(this);
		}

		std::string to_string() override { return "shield"; }
	};
}




