#pragma once
#include "ec/component/system/component-system.h"
#include "ec/component/component-template.h"
#include "ec/component/component-tilemap.h"
#include "ec/component/src/component-src.h"
#include "ec/component/component-vec2.h"
#include <glm/glm.hpp>

namespace System
{
	class Camera : public ISystem
	{
		Component::Vector2D* follower_pos_;
		Component::Vector2D* camera_;
		Component::Tilemap* tilemap_;
		Component::ValTemplate<float>* follower_width_;
		Component::ValTemplate<float>* follower_height_;
		Component::ValTemplate<float>* scale_, *tilemap_scale_;
		Component::ValTemplate<float>* screen_width_;
		Component::ValTemplate<float>* screen_height_;
		Component::ValTemplate<float>* delta_time_;

		float lerp(float a, float b, float f)
		{
			return (a * (1.0 - f)) + (b * f);
		}
	public:
		Camera()
			: follower_pos_(nullptr), camera_(nullptr), screen_width_(nullptr), screen_height_(nullptr), follower_height_(nullptr), 
			scale_(nullptr), tilemap_scale_(nullptr), tilemap_(nullptr), delta_time_(nullptr), follower_width_(nullptr)
		{}


		void init(nlohmann::json json, Entity* game) override
		{
			if (json.contains("camera"))
				camera_ = game->get_nested_component<Component::Vector2D>(json["camera"]);

			if (json.contains("follower_position"))
			{
				follower_width_ = game->get_nested_component<Component::ValTemplate<float>>(json["follower_width"]);
				follower_height_ = game->get_nested_component<Component::ValTemplate<float>>(json["follower_height"]);
				scale_ = game->get_nested_component<Component::ValTemplate<float>>(json["scale"]);
				follower_pos_ = game->get_nested_component<Component::Vector2D>(json["follower_position"]);
			}

			if (json.contains("tilemap"))
			{
				tilemap_ = game->get_nested_component<Component::Tilemap>(json["tilemap"]);
				tilemap_scale_ = game->get_nested_component<Component::ValTemplate<float>>(json["tilemap_scale"]);
			}
			else
			{
				tilemap_ = nullptr;
				tilemap_scale_ = nullptr;
			}
			
				
			screen_width_ = game->get_component<Component::ValTemplate<float>>("width");
			screen_height_ = game->get_component<Component::ValTemplate<float>>("height");
			delta_time_ = game->get_component<Component::ValTemplate<float>>("delta_time");
			camera_->x = follower_pos_->x - screen_width_->val / 2.0f + follower_width_->val / 2.0f * scale_->val;
			camera_->y = follower_pos_->y - screen_height_->val / 2.0f + follower_height_->val / 2.0f * scale_->val;
		}

		void execute() override
		{
			// set camera position so the follower is always at the center
			glm::vec2 follower = {
				follower_pos_->x - screen_width_->val / 2.0f + follower_width_->val / 2.0f * scale_->val,
				follower_pos_->y - screen_height_->val / 2.0f + follower_height_->val / 2.0f * scale_->val
			};

			auto cam_follower_x_diff = std::abs(camera_->x - follower.x);
			auto cam_follower_y_diff = std::abs(camera_->y - follower.y);

			
			camera_->x = lerp(camera_->x, follower.x, cam_follower_x_diff * delta_time_->val);
			camera_->y = lerp(camera_->y, follower.y, cam_follower_y_diff * delta_time_->val);
			
			if (tilemap_)
			{
				if ((camera_->x + screen_width_->val) > tilemap_->width * tilemap_->tile_size * tilemap_scale_->val)
					camera_->x = tilemap_->width * tilemap_->tile_size * tilemap_scale_->val - screen_width_->val;
				else if (camera_->x < 0.0f)
					camera_->x = 0.0f;
				if ((camera_->y + screen_height_->val) > tilemap_->height * tilemap_->tile_size * tilemap_scale_->val)
					camera_->y = tilemap_->height * tilemap_->tile_size * tilemap_scale_->val - screen_height_->val;
				else if ((camera_->y < 0))
					camera_->y = 0;
			}

		}
		std::string get_id() override { return "system-camera"; }
	};

}