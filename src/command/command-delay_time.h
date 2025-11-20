#pragma once
#include "command/command.h"
#include <GLFW/glfw3.h>
#include "ec/component/component-pathway.h"

namespace Command
{
	class DelayTime : public ICommand
	{
		double delay;
		double start_time;
		nlohmann::json next;
	public:
		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
			next = {};
			if (json.contains("time"))
			{
				delay = json["time"];
				next = json["next"];
			}
			else
				delay = json.get<double>();
			start_time = glfwGetTime();
		}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
			if (start_time + delay > glfwGetTime())
				pathway->next(this);
			else
				if (!next.is_null())
					pathway->message(next);
		}
		std::string to_string() override { return "delay_time"; }
	}; 
}