#pragma once
#include "command/command.h"
#include "ec/component/component-box.h"
#include "ec/component/component-template.h"
#include "ec/component/block/component-block-text.h"

namespace Command
{
	class UpdateMessage : public ICommand
	{
		nlohmann::json textbox_;
		std::string message_;
	public:

		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
			textbox_ = json["textbox"];
			message_ = json["message"].get<std::string>();
		}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
			// get blocks
			Entity* textbox = game->get_nested_child(textbox_);
			auto words = textbox->get_component<Block::Words>("block-text");
			auto width = textbox->get_component<Component::ValTemplate<float>>("width");
			auto box_scale = textbox->get_component<Component::ValTemplate<float>>("box_scale");
			auto adjusted_width = width->val * box_scale->val - words->offset.x * 2;
			words->text.write(message_, glm::vec2(0), adjusted_width);
		}
		std::string to_string() override { return "update_message"; }
	};
}