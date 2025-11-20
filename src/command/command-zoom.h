#pragma once
#include "command/command.h"
#include "templates.h"
#include "ec/component/component-template.h"
#include "ec/component/component-vec2.h"
#include "ec/component/component-pathway.h"

namespace Command
{
	class Zoom : public ICommand
	{
		Component::Float* camera_zoom_;
		Component::Position* camera_position_;
		Component::Float* delta_time_;
        bool first_;
        float width_, height_; 
		float end_scale_;
		float scale_difference_;
		float step_;
	public:

		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
			camera_zoom_ = game->get_nested_component<Component::Float>(json["camera_zoom"]); // camera zoom
			camera_position_ = game->get_nested_component<Component::Position>(json["camera_position"]); // camera position
			end_scale_ = json["end_scale"];
			step_ = json["step"];
            width_ = game->get_component<Component::Float>("width")->val;
            height_ = game->get_component<Component::Float>("height")->val;
            first_ = true;
			delta_time_ = game->get_component<Component::Float>("delta_time");
		}

        void execute(Entity*, Component::Pathway* pathway)
        {
            /* ------------------------------------------------------------------ one-time */
            if (first_) {
                scale_difference_ = end_scale_ - camera_zoom_->val;   // signed
                first_ = false;
            }

            /* ------------------------------------------------------------------ 1. pick step */
            const float dt = delta_time_->val;
            const float signed_step = (scale_difference_ > 0.f ? 1.f : -1.f) * step_ * dt;

            /* clamp so we never overshoot */
            const float new_zoom =
                std::fabs(scale_difference_) <= std::fabs(signed_step)
                ? end_scale_
                : camera_zoom_->val + signed_step;

            /* ------------------------------------------------------------------ 2. pan so centre / bottom stay put */
            const float old_zoom = camera_zoom_->val;
            const float old_vp_w_wu = width_ / old_zoom;           // viewport size in world units
            const float old_vp_h_wu = height_ / old_zoom;

            const float world_centre_x = camera_position_->x + old_vp_w_wu * 0.5f;
            const float world_bottom = camera_position_->y + old_vp_h_wu;   // ← anchor this!

            /* apply zoom */
            camera_zoom_->val = new_zoom;

            const float new_vp_w_wu = width_ / new_zoom;
            const float new_vp_h_wu = height_ / new_zoom;

            /* X : keep centre */
            camera_position_->x = world_centre_x - new_vp_w_wu * 0.5f;
            /* Y : keep bottom */
            camera_position_->y = world_bottom - new_vp_h_wu;      // ← THIS LINE CHANGED

            /* ------------------------------------------------------------------ 3. done?  */
            if (std::fabs(new_zoom - end_scale_) > 1e-4f)         // keep going
                pathway->next(this);
        }


		std::string to_string() override { return "zoom"; }
	};
}