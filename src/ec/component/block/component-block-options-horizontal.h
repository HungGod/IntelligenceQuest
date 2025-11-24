#pragma once
#include "ec/component/block/component-block-options.h"

namespace Options
{
	class Horizontal : public AOptions
	{
	public:

		void change_options(nlohmann::json json) override
		{
			options_.clear();

			if (json.is_array())
			{
				Component::Int* line_h = font_->get_component<Component::Int>("line_h");
				float x_pos = 0;

				for (nlohmann::json j : json)
				{
					Text text;
					text.init(font_, text_scale_->val);
					text.write(j.get<std::string>(), glm::vec2(center_.x + x_pos + cursor_src_->get_width() * cursor_scale_, center_.y - line_h->val / 2.0f));
					options_.push_back(AOptions::Option{ text, glm::vec2(center_.x + x_pos, center_.y - line_h->val / 2.0f) });
					x_pos += text.size + cursor_src_->get_width() * cursor_scale_;
				}

				// center the options
				float x_off = x_pos / 2.0f;
				for (AOptions::Option& op : options_)
				{
					for (glm::vec4& d : op.text.dests)
					{
						d.x -= x_off;
						d.z -= x_off;
					}
					op.cursor_position.x -= x_off;
				}

			}
			else
				Logger::error("json is not an array: Component::Blocks::Options::Horizontal", Logger::HIGH);

		}

		std::string get_id() override { return "block-options-horizontal"; }
	};
}