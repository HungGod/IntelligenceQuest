#pragma once
#include "command/command.h"
#include "ec/component/component-template.h"
#include "ec/component/component-pathway.h"

namespace Command
{
	class RepeatedEvent : public ICommand
	{
		std::vector<nlohmann::json> commands_;
		Component::ValTemplate<int>* counter_;
		Component::Pathway* pathway_;
	public:

		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
			if (json["commands"].is_array())
				commands_ = json["commands"].get<std::vector<nlohmann::json>>();
			else
				Logger::error("Json[commands] is not an array", Logger::HIGH);

			counter_ = game->get_nested_component<Component::ValTemplate<int>>(json["counter"]);
			pathway_ = game->get_component<Component::Pathway>("pathway");
		}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
			if (counter_->val >= commands_.size())
				counter_->val = 0;

			pathway_->message(commands_[counter_->val++]);
		}

		std::string to_string() override { return "repeated_event"; }
	};
}