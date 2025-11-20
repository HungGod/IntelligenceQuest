#pragma once
#include <vector>
#include "ec/component/component.h"
#include "switch_ring.h"
#include "ec/component/factory/component-factory-command_map.h"

namespace Component
{
	class Pathway : public IComponent
	{
		SwitchRing<nlohmann::json> messages_;
		SwitchRing<ICommand*> commands_;
	public:
		void message(nlohmann::json message);
		void next(ICommand* comand);
		void navigate(Entity* game, Factory::CommandMap* command_map);
		ICommand* get_command(std::pair<const std::string, nlohmann::json>& obj, Entity* game, Factory::CommandMap* command_map);
		std::string get_id() override { return "pathway"; }
	};
}
