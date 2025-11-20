#pragma once
#include "ec/component/src/component-src.h"
#include "rect.h"
#include <glm/glm.hpp>
#include <unordered_map>

namespace Src
{
	class BMFont : public ISrc
	{
	public:
		int glyph;
		int advance;
		Rect src;
		Rect offset;
		std::unordered_map<int, int> kerning;

		BMFont()
			: glyph(0), advance(0), src{ 0 }, offset{0}
		{}

		void init(nlohmann::json json, Entity* game)
		{
			glyph = json["id"];

			src.x = json["x"];
			src.y = json["y"];
			src.w = json["width"];
			src.h = json["height"];

			offset.x = json["xoffset"];
			offset.y = json["yoffset"];
			offset.w = src.w;
			offset.h = src.h;

			advance = json["xadvance"];
		}

		glm::vec4 get_dest(glm::vec2 position, float scale) override final
		{
			return glm::vec4{
				position.x + offset.x * scale,
				position.y + offset.y * scale,
				position.x + offset.x * scale + src.w * scale,
				position.y + offset.y * scale + src.h * scale
			};
		}

		glm::vec4 get_src() override final
		{
			return glm::vec4(src.x, src.y, src.x + src.w, src.y + src.h);
		}

		float get_width() override { return offset.w; }
		float get_height() override { return offset.h; }

		void normalize(float img_width, float img_height) override final
		{
			src.x /= img_width;
			src.y /= img_height;
			src.w /= img_width;
			src.h /= img_height;
		}

		std::string get_id() override { return "src-bitmap_glyph"; }
	};
}