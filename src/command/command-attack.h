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
	class Attack : public ICommand
	{
		// Player references
		Component::Vector2D* position_;
		Component::ValTemplate<float>* scale_;
		Component::ValTemplate<float>* width_;
		Component::Vector2D* direction_;
		
		// Attack entity references
		Component::Vector2D* attack_position_;
		Src::Transform* attack_src_;
		Component::Collider* attack_collider_;
		
		// Combo hit data (preloaded)
		struct ComboHit {
			double duration;
			double startup;
			float damage;
			float hitstun;
			glm::vec2 knockback;
			glm::vec2 impulse;
			IGJK* hitbox_left;
			IGJK* hitbox_right;
			ISrc* frame_left;
			ISrc* frame_right;
		};
		std::vector<ComboHit> combo_hits_;
		
		// State
		int current_hit_;
		double hit_start_time_;
		bool hitbox_active_;
		float facing_direction_;

		void update_attack_position()
		{
			if (!attack_position_ || !position_) return;
			attack_position_->x = position_->x;
			attack_position_->y = position_->y;
		}

		void activate_hitbox(const ComboHit& hit)
		{
			if (!attack_collider_) return;
			
			attack_collider_->shapes.clear();
			
			IGJK* hitbox = (facing_direction_ >= 0) ? hit.hitbox_right : hit.hitbox_left;
			if (hitbox)
				attack_collider_->shapes.push_back(hitbox);
			
			// Set hit data on collider
			nlohmann::json hit_data;
			hit_data["damage"] = hit.damage;
			hit_data["hitstun"] = hit.hitstun;
			hit_data["knockback"]["x"] = hit.knockback.x * facing_direction_;
			hit_data["knockback"]["y"] = hit.knockback.y;
			attack_collider_->data = hit_data;
		}

		void deactivate_hitbox()
		{
			if (attack_collider_)
			{
				attack_collider_->shapes.clear();
				attack_collider_->data = nlohmann::json{};
			}
		}

		void update_sprite(const ComboHit& hit)
		{
			if (!attack_src_) return;
			ISrc* frame = (facing_direction_ >= 0) ? hit.frame_right : hit.frame_left;
			if (frame)
				{
					attack_src_->src = frame;
				}
		}

		void apply_impulse(const glm::vec2& impulse)
		{
			if (position_ && (impulse.x != 0.0f || impulse.y != 0.0f))
			{
				position_->x += impulse.x * facing_direction_;
				position_->y += impulse.y;
			}
		}

	public:
		Attack() = default;

		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
			// Load player references
			position_ = game->get_nested_component<Component::Vector2D>(json["position"]);
			scale_ = game->get_nested_component<Component::ValTemplate<float>>(json["scale"]);
			direction_ = game->get_nested_component<Component::Vector2D>(json["direction"]);
			width_ = game->get_nested_component<Component::ValTemplate<float>>(json["width"]);
			
			// Load attack entity references
			attack_position_ = game->get_nested_component<Component::Vector2D>(json["attack_position"]);
			attack_src_ = game->get_nested_component<Src::Transform>(json["attack_src"]);
			attack_collider_ = game->get_nested_component<Component::Collider>(json["attack_collider"]);
			
			// Get spritesheet for frames
			Entity* spritesheet = game->get_nested_child(json["spritesheet"]);
			
			// Load combo hits
			combo_hits_.clear();
			
			if (json.contains("combo"))
			{
				for (auto& hit_json : json["combo"])
				{
					ComboHit hit;
					hit.duration = hit_json.value("duration", 0.2);
					hit.startup = hit_json.value("startup", 0.05);
					hit.damage = hit_json.value("damage", 10.0f);
					hit.hitstun = hit_json.value("hitstun", 0.2f);
					
					hit.knockback = {
						hit_json["knockback"].value("x", 100.0f),
						hit_json["knockback"].value("y", 0.0f)
					};
					hit.impulse = {
						hit_json["impulse"].value("x", 0.0f),
						hit_json["impulse"].value("y", 0.0f)
					};
					
					// Load preloaded hitbox shapes
					hit.hitbox_left = hit_json.contains("hitbox_left") ? 
						game->get_nested_component<IGJK>(hit_json["hitbox_left"]) : nullptr;
					hit.hitbox_right = hit_json.contains("hitbox_right") ? 
						game->get_nested_component<IGJK>(hit_json["hitbox_right"]) : nullptr;
					
					// Load sprite frames from spritesheet
					hit.frame_left = (spritesheet && hit_json.contains("frame_left")) ?
						spritesheet->get_component<ISrc>(hit_json["frame_left"].get<std::string>()) : nullptr;
					hit.frame_right = (spritesheet && hit_json.contains("frame_right")) ?
						spritesheet->get_component<ISrc>(hit_json["frame_right"].get<std::string>()) : nullptr;
					
					combo_hits_.push_back(hit);
				}
			}
			else
			{
				// Single attack format
				ComboHit hit;
				hit.duration = json.value("duration", 0.25);
				hit.startup = json.value("startup", 0.05);
				hit.damage = json.value("damage", 10.0f);
				hit.hitstun = json.value("hitstun", 0.2f);
				
				hit.knockback = {
					json["knockback"].value("x", 100.0f),
					json["knockback"].value("y", 0.0f)
				};
				hit.impulse = {
					json["impulse"].value("x", 0.0f),
					json["impulse"].value("y", 0.0f)
				};
				
				
				hit.hitbox_left = json.contains("hitbox_left") ?
					game->get_nested_component<IGJK>(json["hitbox_left"]) : nullptr;
				hit.hitbox_right = json.contains("hitbox_right") ?
					game->get_nested_component<IGJK>(json["hitbox_right"]) : nullptr;
				
				hit.frame_left = (spritesheet && json.contains("frame_left")) ?
					spritesheet->get_component<ISrc>(json["frame_left"].get<std::string>()) : nullptr;
				hit.frame_right = (spritesheet && json.contains("frame_right")) ?
					spritesheet->get_component<ISrc>(json["frame_right"].get<std::string>()) : nullptr;
				
				combo_hits_.push_back(hit);
			}
			
			// Initialize state
			current_hit_ = 0;
			hit_start_time_ = glfwGetTime();
			hitbox_active_ = false;
			facing_direction_ = (direction_->x >= 0) ? 1.0f : -1.0f;
			
			// Apply first hit impulse
			if (!combo_hits_.empty())
			{
				apply_impulse(combo_hits_[0].impulse);
				update_sprite(combo_hits_[0]);
			}
		}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
			if (combo_hits_.empty() || current_hit_ >= static_cast<int>(combo_hits_.size()))
			{
				if (attack_src_) attack_src_->src = nullptr;
				deactivate_hitbox();
				return;
			}
			
			ComboHit& hit = combo_hits_[current_hit_];
			double elapsed = glfwGetTime() - hit_start_time_;
			
			update_attack_position();
			update_sprite(hit);
			
			// Activate hitbox after startup
			if (!hitbox_active_ && elapsed >= hit.startup)
			{
				hitbox_active_ = true;
				activate_hitbox(hit);
			}
			
			// Move to next hit when duration ends
			if (elapsed >= hit.duration)
			{
				deactivate_hitbox();
				hitbox_active_ = false;
				current_hit_++;
				hit_start_time_ = glfwGetTime();
				
				if (current_hit_ < static_cast<int>(combo_hits_.size()))
					apply_impulse(combo_hits_[current_hit_].impulse);
			}
			
			// Continue or finish
			if (current_hit_ < static_cast<int>(combo_hits_.size()))
				pathway->next(this);
			else
			{
				if (attack_src_) attack_src_->src = nullptr;
			}
		}

		std::string to_string() override { return "attack"; }
	};
}
