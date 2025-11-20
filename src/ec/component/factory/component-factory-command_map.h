#pragma once
#include "ec/component/factory/component-factory.h"
#include "ring.h"
#include <string>
#include <unordered_map>
#include "command/command.h"

namespace Factory
{
	struct CommandMap : public IFactory, public std::unordered_map<std::string, std::shared_ptr<Ring<ICommand>>>
	{
		void create() override;

		std::string get_id() override { return "factory-command_map"; }
	};
}