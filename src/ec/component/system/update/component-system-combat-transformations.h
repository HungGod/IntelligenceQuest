/****************************************************************************************
 * combat_transformations.cpp  –  camera-driven stage pan & zoom
 * ------------------------------------------------------------------
 *  • Fighters remain in world coordinates.
 *  • The system updates just two things:
 *        camera_pos_->x   (world-space pan)
 *        camera_zoom_->val (uniform zoom)
 *  • Zoom is damped; panning keeps both farthest fighters visible.
 ****************************************************************************************/
#pragma once
#include <algorithm>
#include <cmath>
#include <vector>
#include <cfloat>
#include "ec/component/system/component-system.h"
#include "ec/component/component-template.h"
#include "ec/entity.h"
#include "ec/component/component-vec2.h"

namespace System {

    template <class T, class U, class V>
    constexpr auto clamp(T v, U lo, V hi) -> std::common_type_t<T, U, V>
    {
        using R = std::common_type_t<T, U, V>;
        const R val(v), l(lo), h(hi);
        return val < l ? l : h < val ? h : val;
    }

    class CombatTransformations : public ISystem
    {
        struct Fighter {
            Component::Position* pos{};
            Component::Bool* stunned{};
            Component::Float* width{};
            Component::Float* height{};
            Component::Velocity* velocity{};
        };

        std::vector<Fighter> fighters_;

        /* camera components supplied from JSON */
        Component::Position* camera_pos_{};
        Component::Float* camera_zoom_{};

        /* stage / engine constants */
        float  max_zoom_{ 1.f };
        float  min_zoom_{ 1.f };
        float  lag_frames_{ 8.f };

        float zoom_out_percent_{ 0.9f };
        float zoom_in_percent_{ 0.6f };

        Component::Float* viewport_width_{};   // in screen pixels
        Component::Float* viewport_height_{};   // in screen pixels
        Component::Float* delta_time_{};       // seconds
    public:
        /* ISystem interface */
        void init(nlohmann::json j, Entity* g) override;
        void execute() override;
        std::string get_id() override { return "system_combat_transformations"; }
    private:
        void zoom_to(float target, float dt);
    };

    /* -------------------------------------------------------------------- init */
    void CombatTransformations::init(nlohmann::json j, Entity* g)
    {
        camera_pos_ = g->get_nested_component<Component::Position>(j["camera_position"]);
        camera_zoom_ = g->get_nested_component<Component::Float>(j["camera_zoom"]);
        max_zoom_ = j["max_zoom"].get<float>();
        min_zoom_ = j["min_zoom"].get<float>();
        lag_frames_ = j["lag_frames"].get<float>();
        zoom_out_percent_ = j["zoom_out_percent"].get<float>();
        zoom_in_percent_ = j["zoom_in_percent"].get<float>();

        for (auto& jc : j["combatants"]) {
            Fighter f;
            f.pos = g->get_nested_component<Component::Position>(jc["position"]);
            f.stunned = g->get_nested_component<Component::Bool>(jc["stunned"]);
            f.width = g->get_nested_component<Component::Float>(jc["width"]);
            f.height = g->get_nested_component<Component::Float>(jc["height"]);
            f.velocity = g->get_nested_component<Component::Velocity>(jc["velocity"]);
            fighters_.push_back(f);
        }

        viewport_width_ = g->get_component<Component::Float>("width");
        viewport_height_ = g->get_component<Component::Float>("height");
        delta_time_ = g->get_component<Component::Float>("delta_time");
    }

    /* -------------------------------------------- helper: critically damped zoom */
    void CombatTransformations::zoom_to(float target, float dt)
    {
        target = clamp(target, min_zoom_, max_zoom_);
        const float alpha = 1.f - std::exp(-dt * lag_frames_);      
        camera_zoom_->val += (target - camera_zoom_->val) * alpha;
    }

    /* ------------------------------------------------------------------ execute */
    void CombatTransformations::execute()
    {
        const float dt = delta_time_->val;
        const float vp_w = viewport_width_->val;
        const float vp_h = viewport_height_->val;
        const float old_zoom = camera_zoom_->val;

        /* 2. bounding box of fighters (world units) */
        float min_x = FLT_MAX, max_x = -FLT_MAX;
        for (auto& f : fighters_) {
            min_x = std::min(min_x, f.pos->x);
            max_x = std::max(max_x, f.pos->x + f.width->val);
        }
        const float span_world = max_x - min_x;

        /* 3. choose zoom so the group fits % of the screen width */
        float zoom_goal = camera_zoom_->val;
        const float span_screen = span_world * camera_zoom_->val;
        const float upper_px = vp_w * zoom_out_percent_;
        const float lower_px = vp_w * zoom_in_percent_;

        if (span_screen > upper_px)
            zoom_goal = upper_px / span_world;      // zoom out
        else if (span_screen < lower_px)
            zoom_goal = lower_px / span_world;      // zoom in

        zoom_to(zoom_goal, dt);

        /* 4. pan camera so fighters’ centre stays at screen centre */
        const float world_centre = (min_x + max_x) * 0.5f;
        camera_pos_->x = world_centre - (vp_w * 0.5f) / camera_zoom_->val;


        /* 4. anchor bottom*/
        const float world_bottom = camera_pos_->y + vp_h / old_zoom;
        camera_pos_->y = world_bottom - vp_h / camera_zoom_->val;
    }

} // namespace System
