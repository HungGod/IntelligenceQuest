#pragma once
#include "ec/component/src/component-src.h"
#include "rect.h"
#include <glm/glm.hpp>
#include "logger.h"

namespace Src
{
	class TexturePackerGUI : public ISrc
	{
	public:
		Rect src;
		Rect offset;

		TexturePackerGUI()
			: src{ 0 }, offset()
		{}

		void init(nlohmann::json json, Entity* game) override final
		{
			src.x = json["frame"]["x"];
			src.y = json["frame"]["y"];
			src.w = json["frame"]["w"];
			src.h = json["frame"]["h"];

			offset.x = json["spriteSourceSize"]["x"];
			offset.y = json["spriteSourceSize"]["y"];
			offset.w = json["spriteSourceSize"]["w"];
			offset.h = json["spriteSourceSize"]["h"];
		}

		glm::vec4 get_dest(glm::vec2 position, float scale) override final
		{
			return glm::vec4{
				position.x + offset.x * scale,
				position.y + offset.y * scale,
				position.x + offset.x * scale + offset.w * scale,
				position.y + offset.y * scale + offset.h * scale
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

		std::string get_id() override final { return "src-texture_packer_gui"; }
	};
}