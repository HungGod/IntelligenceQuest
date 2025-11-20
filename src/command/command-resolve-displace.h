#pragma once
#include "command/command.h"
#include "ec/component/collider/component-collider.h"


namespace Resolve
{
	class Displace : public ICommand
	{
		Component::Collider* col_a_, * col_b_;
	public:

		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
			col_a_ = game->get_nested_component<Component::Collider>(json["col_a"]["collider"]);
			col_b_ = game->get_nested_component<Component::Collider>(json["col_b"]["collider"]);
		}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
			glm::vec2 piercing_vec = col_a_->piercing_vec(col_b_);

			col_b_->moveable ? *col_a_->position += piercing_vec / 2.f : *col_a_->position += piercing_vec;
		}
		std::string to_string() override { return "resolve-displace"; }
	};
}