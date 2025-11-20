#include "ec/component/component-pathway.h"
#include "command/command.h"

void Component::Pathway::message(nlohmann::json message)
{
	if (!message.is_array())
		messages_.insert_next(message);
	else
		messages_.insert_all_next(message);
}

void Component::Pathway::next(ICommand* command)
{
	commands_.insert_next(command);
	command->use();
}
void Component::Pathway::navigate(Entity* game, Factory::CommandMap* command_map)
{
	for (auto& c : commands_.get())
	{
		c->discard();
		c->execute(game, this);
	}
	auto next_messages = messages_.get();
	
	for (auto& message : next_messages)
	{
		auto objects = message.get<nlohmann::json::object_t>();
		for (auto& obj : objects)
			this->next(get_command(obj, game, command_map));
	}
}

ICommand* Component::Pathway::get_command(std::pair<const std::string, nlohmann::json>& obj, Entity* game, Factory::CommandMap* command_map)
{
	if (!command_map->count(obj.first))
		Logger::error("unable to find command: " + obj.first + "\n" + obj.second.dump(1, '\t'), Logger::HIGH);

	auto command = (*command_map)[obj.first]->get();
	command->load(obj.second, game, this);
	return command;
}
