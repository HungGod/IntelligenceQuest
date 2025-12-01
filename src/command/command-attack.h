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
        nlohmann::json attack_shape_;
        bool attack_init_;
        glm::vec2 offset_;
        nlohmann::json attack_data_;
	public:

		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
                  hitboxes_ = game->get_nested_component<Component::Hitboxes>(json["hitboxes"]);
                  direction_ = game->get_nested_component<Component::Vector2D>(json["direction"]);
                  position_ = game->get_nested_component<Component::Vector2D>(json["position"]);
                  scale_ = game->get_nested_component<Component::ValTemplate<float>>(json["scale"]);
                  height_ = game->get_nested_component<Component::ValTemplate<float>>(json["height"]);
                  width_ = game->get_nested_component<Component::ValTemplate<float>>(json["width"]);

                  glm::vec2 max {width_->val, height_->val};
                  glm::vec2 min{0.0f, 0.0f};
                  attack_init_ = false;
		}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
                  // I need to create a collider and add it to hitboxes. Change data to include the attack information/ stun information
                  if (hitboxes_->shapes.size() == 0 && attack_init_ == false)
                  {
                        hitboxes_->position = position_;
                        hitboxes_->scale = scale_;
                        IGJK* attack_gjk = hitboxes_->create_shape(attack_shape);
                        attack_gjk->offset = offset_;
                        hitboxes_->shapes.push_back(hitbox);
                        hitboxes_->data = attack_data_;
                        attack_init_ = true;
                  }
                  else if (attack_init_ = true)
                  {

                  }
                  

		}
		std::string to_string() override { return "attack"; }
	};
}