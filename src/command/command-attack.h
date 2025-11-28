#pragma once
#include "command/command.h"
#include "ec/component/component-template.h"
#include "ec/component/component-hitboxes.h"

namespace Command
{
	class Attack : public ICommand
	{
        Component::Vector2D* position_;
        Component::ValTemplate<float>* scale_;
        Component::ValTemplate<float>* height_;
        Component::ValTemplate<float>* width_;
        Component::Hitboxes* hitboxes_;
        Component::Vector2D* direction_;
	public:

		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
            hitboxes_ = game->get_nested_component<Component::Hitboxes>(json["hitboxes"]);
            direction_ = game->get_nested_component<Component::Vector2D>(json["direction"]);
            position_ = game->get_nested_component<Component::Vector2D>(json["position"]);
            scale_ = game->get_nested_component<Component::ValTemplate<float>>(json["scale"]);
            height_ = game->get_nested_component<Component::ValTemplate<float>>(json["height"]);
            width_ = game->get_nested_component<Component::ValTemplate<float>>(json["width"]);
		}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
		}
		std::string to_string() override { return "attack"; }
	};
}