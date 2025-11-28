#pragma once
#include "command/command.h"
#include <unordered_map>
#include "ec/entity.h"
#include <sstream>
#include "ec/component/component-pathway.h"

namespace Command
{
	class LoadFiles : public ICommand
	{
		nlohmann::json json_;
	public:

		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
			json_ = json;
		}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
			auto& json_map = *game->get_component<Component::Template<std::unordered_map<std::string, nlohmann::json>>>("json_map");
			std::vector<std::string> filenames_;
			bool reload = false;

			if (!json_.is_array())
			{
				if (json_.contains("gamestate"))
				{
					auto current_gamestate = game->get_component<Component::ValTemplate<nlohmann::json>>("curr_state");
					current_gamestate->val = json_;
					filenames_.push_back(json_["gamestate"]);
					reload = true;
				}
				else if (json_.contains("reload"))
				{
					if (json_["files"].is_array())
						filenames_ = json_["files"].get<std::vector<std::string>>();
					else
						filenames_.push_back(json_["files"]);
					reload = true;
				}
				else
					filenames_.push_back(json_);
			}
			else
				filenames_ = json_.get<std::vector<std::string>>();

			for (auto fn : filenames_)
			{
				nlohmann::json parsed_message;
				if (json_map.count(fn))
				{
					if (reload)
						parsed_message = json_map[fn];
					else
						continue;
				}
				else
				{
					std::stringstream json_stream;

					try
					{
						// open files
						std::ifstream indx_file(fn);

						// read into temp string streams
						json_stream << indx_file.rdbuf();

						// close file streams
						indx_file.close();
					}
					catch (std::exception e) {
						Logger::error("Failed to read json file! path = " + fn, Logger::HIGH);
					}
					try
					{
						parsed_message = nlohmann::json::parse(json_stream);
					}
					catch (std::exception e)
					{
						Logger::error("couldn't parse json file: " + fn + "\n\n json: \n" + json_stream.str(), Logger::HIGH);
					}

					json_map[fn] = parsed_message;
				}


				if (json_.contains("layer"))
				{
					JsonTo json_to;
					parsed_message = json_to.vector(parsed_message)[json_["layer"].get<int>()];
				}

				pathway->message(parsed_message);
			}
		}
		std::string to_string() override { return "load_files"; }
	};
}