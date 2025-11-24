#pragma once
#include "command/command.h"
#include "ec/component/component-vec2.h"
#include "ec/component/block/component-block-box.h"

namespace Command
{
	class UpdateSpeechArrow : public ICommand
	{
		nlohmann::json player_;
		nlohmann::json obj_;
		nlohmann::json box_;
		bool no_speech_arrow_;
	public:
		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
			player_ = json["player"];
			obj_ = json["obj"];
			box_ = json["box"];
			no_speech_arrow_ = json["no_speech_arrow"];
		}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
			Component::Position* player = game->get_nested_component<Component::Position>(player_);
			Component::Position* obj = game->get_nested_component<Component::Position>(obj_);
			Block::Box* box = game->get_nested_component<Block::Box>(box_);

			bool player_is_left_of_obj = (player->x - obj->x) > 0;
			box->change_arrow(player_is_left_of_obj, no_speech_arrow_);
		}
		std::string to_string() override { return "update_speech_arrow"; }
	};
}
