#pragma once
#include "ec/component/src/component-src.h"
#include "ec/entity.h"
#include "ec/component/src/component-src.h"

namespace Src
{
	class FlexOutdated : public ISrc
	{
	public:
		ISrc* src;

		FlexOutdated()
			: src(nullptr)
		{}

		void init(nlohmann::json json, Entity* game)
		{
			if (json.contains("path"))
				src = game->get_nested_component<ISrc>(json["path"]);
			else if (!json.is_null())
				src = game->get_nested_component<ISrc>(json);
			else
				src = nullptr;
		}

		void normalize(float img_width, float img_height) override final
		{
			if (!src)
				Logger::error("trying to normalize an unset src", Logger::MEDIUM);

			src->normalize(img_width, img_height);
		}

		glm::vec4 get_dest(glm::vec2 position, float scale) override final
		{
			if (!src)
				Logger::error("trying to get_dest an unset src", Logger::HIGH);

			return src->get_dest(position, scale);
		}

		glm::vec4 get_src()
		{
			if (!src)
				Logger::error("trying to get_src an unset src", Logger::HIGH);

			return src->get_src();
		}

		float get_width()
		{
			if (!src)
				Logger::error("trying to get_width on an unset src", Logger::HIGH);

			return src->get_width();
		}

		float get_height()
		{
			if (!src)
				Logger::error("trying to get_height on an unset src", Logger::HIGH);

			return src->get_height();
		}

		std::string get_id() override { return "src-flex"; }
	};
}