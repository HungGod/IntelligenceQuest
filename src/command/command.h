#pragma once
#include <json.hpp>
#include "logger.h"
#include "ec/entity.h"

namespace Component
{
	class Pathway;
}

class ICommand 
{
	bool in_use_;

friend class Component::Pathway;

	void use() {
		if (in_use_)
			Logger::error("Command in use", Logger::HIGH);

		in_use_ = true;
	}

	void discard() { in_use_ = false; }

public:
	ICommand()
		: in_use_(false)
	{}

	bool is_in_use() { return in_use_; }
	virtual void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) = 0;
	virtual void execute(Entity* game, Component::Pathway* pathway) = 0;
	virtual std::string to_string() = 0;
};

