#pragma once
#include "ec/component/block/component-block-options.h"

namespace Options
{
	class Vertical : public AOptions
	{
	public:

		void change_options(nlohmann::json json) override
		{
			options_.clear();

			if (json.is_array())
			{
				int line_h = font_->get_component<Component::Int>("line_h")->val * text_scale_->val;
				float y_pos = 0.0f;
				float x_pos = 0.0f;
				for (nlohmann::json j : json)
				{
					Text text;
					text.init(font_, text_scale_->val);
					text.write(j.get<std::string>(), glm::vec2(center_.x + cursor_src_->get_width() * cursor_scale_, center_.y + y_pos));
					options_.push_back(AOptions::Option{ text, glm::vec2(center_.x, center_.y + y_pos) });
					y_pos += line_h;
					if (text.size > x_pos)
						x_pos = text.size;
				}

				// center the options
				float y_off = (y_pos - line_h) / 2.0f;
				float x_off = x_pos / 2.0f;
				for (AOptions::Option& op : options_)
				{
					for (glm::vec4& d : op.text.dests)
					{
						d.x -= x_off;
						d.z -= x_off;
						d.y -= y_off;
						d.w -= y_off;
					}
					op.cursor_position.x -= x_off;
					op.cursor_position.y -= y_off;
				}
			}
			else
				Logger::error("json is not an array: Component::Blocks::Options::Vertical", Logger::HIGH);
		}

		std::string get_id() override { return "block-options-vertical"; }
	};
}