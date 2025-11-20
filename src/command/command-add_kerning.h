#pragma once
#include "command/command.h"
#include "ec/component/src-bm_font/component-src-bm_font.h"

namespace Command
{
	class AddKerning : public ICommand
	{
		nlohmann::json path_;
		nlohmann::json kerning_;
	public:

		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
			path_ = json["path"];
			kerning_ = json["kerning"];
		}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
			Entity* font = game->get_nested_child(path_);
			for (nlohmann::json ker : kerning_)
			{
				int first = ker["first"];
				int second = ker["second"];
				int amount = ker["amount"];
				Src::BMFont* glyph = font->get_component<Src::BMFont>(first);
				glyph->kerning[second] = amount;
			}
		}
		std::string to_string() override { return "add_kerning"; }
	};
}