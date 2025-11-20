#pragma once
#include "command/command.h"
#include "ec/component/component-pathway.h"

namespace Command
{
	class DelayCycles : public ICommand
	{
		nlohmann::json next_;
		int cycles_;
	public:
		DelayCycles()
			: cycles_(0)
		{}

		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
			cycles_ = 0;
			if (json.contains("cycles"))
			{
				cycles_ = json["cycles"];
				next_ = json["next"];
			}
			else
				next_ = json;
		}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
			if (cycles_-- <= 0)
				pathway->message(next_);
			else
				pathway->next(this);
		}
		std::string to_string() override { return "delay_cycles"; }
	};
}

