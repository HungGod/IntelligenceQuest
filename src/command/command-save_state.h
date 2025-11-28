#pragma once
#include "command/command.h"
#include "ec/component/component-vec2.h"
#include <sstream>
#include <fstream>
#include "ec/component/component-intelligence.h"

namespace Command
{
	class SaveState : public ICommand
	{
	public:
		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override {}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
			Entity* player = game->get_child("Player");
			auto player_pos = player->get_component<Component::Vector2D>("position");
			auto player_int = player->get_component<Component::Intelligence>("intelligence");
			auto player_flags = player->get_component<Component::ValTemplate<nlohmann::json>>("flags");
			auto curr_gamestate = game->get_component<Component::ValTemplate<nlohmann::json>>("curr_state");

			nlohmann::json save_json;
			save_json["ParseJsonTemplate"]["template"] = "res/data/templates/save.tmpl";
			save_json["ParseJsonTemplate"]["args"]["PLAYER_POS"]["x"] = player_pos->x;
			save_json["ParseJsonTemplate"]["args"]["PLAYER_POS"]["y"] = player_pos->y;
			save_json["ParseJsonTemplate"]["args"]["PLAYER_INT"] = player_int->convert_to_json();
			save_json["ParseJsonTemplate"]["args"]["PLAYER_FLAGS"] = player_flags->val;
			save_json["ParseJsonTemplate"]["args"]["GAMESTATE"] = curr_gamestate->val["gamestate"];
			save_json["ParseJsonTemplate"]["args"]["GAMESTATE_POS"] = curr_gamestate->val["layer"].empty() ? 0 : curr_gamestate->val["layer"].get<int>();

			try
			{
				// open files
				std::ofstream indx_file("res/data/save.json");

				indx_file << save_json.dump(1, '\t');

				// close file streams
				indx_file.close();
			}
			catch (std::exception e) {
				Logger::error("Failed to write to save json file!", Logger::HIGH);
			}

			std::cout << "Saved" << std::endl;
		}

		std::string to_string() override { return "save_state"; }
	};
}