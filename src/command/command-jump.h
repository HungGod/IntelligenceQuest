#pragma once
#include "command/command.h"
#include "templates.h"
#include "ec/component/component-template.h"
#include "ec/component/component-vec2.h"

namespace Command
{
	class Jump : public ICommand
	{
        int jump_velocity_;
        Component::Velocity* velocity_;
		int jump_frames_;
		int initial_jump_frames_;
		float damping_factor_;
	public:

		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
			jump_velocity_ = json["jump_velocity"].get<int>();
			damping_factor_ = json.value("damping_factor", 0.0f);
            velocity_ = game->get_nested_component<Component::Velocity>(json["velocity"]);
			jump_frames_ = json["jump_frames"].get<int>();
			initial_jump_frames_ = jump_frames_;
		}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
			// Calculate damping based on remaining frames
			// ratio goes from 1.0 (beginning) to near 0.0 (end)
			float ratio = static_cast<float>(jump_frames_) / static_cast<float>(initial_jump_frames_);
			// Apply damping: full velocity at start, reduced velocity at end
			float damped_velocity = jump_velocity_ * (1.0f - damping_factor_ * (1.0f - ratio));
            velocity_->y += static_cast<int>(damped_velocity);
            jump_frames_--;
            if (jump_frames_ <= 0)
                pathway->next(this);
		}

		std::string to_string() override { return "jump"; }
	};
}