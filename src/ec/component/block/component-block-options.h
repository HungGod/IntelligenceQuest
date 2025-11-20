#pragma once
#include "ec/component/block/component-block.h"
#include "text.h"

class AOptions : public IBlock
{
	IMaterial* cursor_material_;
	Component::Renderer* renderer_;
	int option_pos_;
protected:
	Component::Float* text_scale_;
	Entity* font_;
	ISrc* cursor_src_;
	float cursor_scale_;
	glm::vec2 center_;
	Component::Int* nav_pos_;
	struct Option
	{
		Text text;
		glm::vec2 cursor_position;
	};
	std::vector<Option> options_;
public:
	AOptions()
		: cursor_src_(nullptr)
	{}

	void init(nlohmann::json json, Entity* game) override final
	{
		options_.clear();
		// find the center of the box
		Component::Float* width = game->get_nested_component<Component::Float>(json["block_width"]);
		Component::Float* height = game->get_nested_component<Component::Float>(json["block_height"]);
		Component::Float* scale = game->get_nested_component<Component::Float>(json["block_scale"]);

		center_ = { width->val * scale->val / 2.0f, height->val * scale->val / 2.0f };

		// load in the cursor and font stuff
		text_scale_ = game->get_nested_component<Component::Float>(json["text_scale"]);
		cursor_src_ = game->get_nested_component<ISrc>(json["cursor_src"]);
		font_ = game->get_nested_child(json["font"]);
		cursor_material_ = game->get_nested_component<IMaterial>(json["cursor_material"]);
		cursor_scale_ = json["cursor_scale"];
		nav_pos_ = game->get_nested_component<Component::Int>(json["nav_pos"]);
	}

	void draw(glm::vec2 position, Component::Renderer* renderer) override final
	{
		for (AOptions::Option& op : options_)
			op.text.draw(position, renderer);
		
		if (options_.size())
		{
			if (nav_pos_->val > options_.size())
				Logger::error("mismatch between options and navigation.", Logger::HIGH);

			AOptions::Option& current_option = options_[nav_pos_->val];
			cursor_src_->draw(current_option.cursor_position + position, cursor_scale_, cursor_material_, renderer);
		}
	}

	virtual void change_options(nlohmann::json json) = 0;
};
