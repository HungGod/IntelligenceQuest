#pragma once
#include "command/command.h"
#include "templates.h"
#include "ec/component/component-template.h"
#include "ec/component/component-hitboxes.h"

namespace Command
{
	class Attack : public ICommand
	{
        Component::Position* position_;
        Component::Float* scale_;
        Component::Float* height_;
        Component::Float* width_;
        Component::Hitboxes* hitboxes_;
        Component::Direction* direction_;
	public:

		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
            hitboxes_ = game->get_nested_component<Component::Hitboxes>(json["hitboxes"]);
            direction_ = game->get_nested_component<Component::Direction>(json["direction"]);
            position_ = game->get_nested_component<Component::Position>(json["position"]);
            scale_ = game->get_nested_component<Component::Float>(json["scale"]);
            height_ = game->get_nested_component<Component::Float>(json["height"]);
            width_ = game->get_nested_component<Component::Float>(json["width"]);
		}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
		}
		std::string to_string() override { return "attack"; }
	};
}