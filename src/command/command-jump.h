#pragma once
#include "command/command.h"
#include "templates.h"
#include "ec/component/component-template.h"


namespace Command
{
	class Jump : public ICommand
	{
        int jump_height_;
        Component::Vector2D* velocity_;
	public:

		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
			jump_height_ = json["jump_height"].get<int>();
            velocity_ = game->get_nested_component<Component::Float>(json["player_velocity"]);
		}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
            // add fancier stuff here later
            velocity_->val.y += jump_height_;
		}

		std::string to_string() override { return "jump"; }
	};
}