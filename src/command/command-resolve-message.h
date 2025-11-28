#pragma once
#include "command/command.h"
#include "glfw/glfw3.h"
#include "ec/component/component-template.h"
#include "ec/component/component-vec2.h"
#include "ec/component/gjk/component-gjk.h"
#include "ec/component/component-pathway.h"

namespace Resolve {
	class Message : public ICommand
	{
		nlohmann::json message_;
		Component::ValTemplate<double>* time_stamp_;
		double delay_;
	public:

		void init(nlohmann::json json, Entity* game)
		{
			message_ = json["col_b"]["message"];
			time_stamp_ = game->get_nested_component<Component::ValTemplate<double>>(json["col_b"]["time_stamp"]);
			delay_ = json["col_b"]["delay"];
			time_stamp_->val = glfwGetTime();

		}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
			double curr_time = glfwGetTime();
			if (curr_time - time_stamp_->val >= delay_)
			{
				pathway->message(message_);
				time_stamp_->val = curr_time;
			}
		}

		std::string to_string() override { return "resolve-message"; }
	};
}