#pragma once
#include "ec/component/component-json_template_map.h"
#include "command/command.h"
#include "ec/component/component-pathway.h"

namespace Command
{
	class ParseJsonTemplate : public ICommand
	{
		nlohmann::json json_;
	public:
		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
			json_ = json;
		}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
			auto& json_template_map = *game->get_component<Component::JsonTemplateMap>("json_template_map");
			nlohmann::json args;

			if (json_.contains("args"))
				args.update(json_["args"]);
			
			if (json_.contains("get"))
				args.update(game->get_nested_component<Component::Json>(json_["get"])->val);

			if (json_template_map.find(json_["template"]) != json_template_map.end())
			{
				auto loaded_template = json_template_map[json_["template"]]->load_template(args);

				pathway->message(loaded_template);
			}
			else
			{
				Logger::error("Template not found: " + json_["template"].get<std::string>(), Logger::HIGH);
			}
		}
		std::string to_string() override { return "parse_json_template"; }
	};
}