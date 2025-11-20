#pragma once
#include "ec/component/gjk/component-gjk.h"
#include "rect.h"
#include "ec/entity.h"
#include "templates.h"
#include "ec/component/component-template.h"

namespace GJK
{
	class Point : public IGJK
	{
		float distance_;
		Component::Direction* direction_;
		glm::vec2 offset_;
	public:
		Point()
			: distance_(0), direction_(nullptr), offset_()
		{}

		void init(nlohmann::json json, Entity* game)
		{
			if (json.contains("direction"))
			{
				direction_ = game->get_nested_component<Component::Direction>(json["direction"]);
				distance_ = json["distance"];
				offset_.x = json["offset"]["x"];
				offset_.y = json["offset"]["y"];
			}

			if (json.contains("scale"))
			{
				auto scale = game->get_nested_component<Component::Float>(json["scale"]);
				distance_ *= scale->val;
			}
			
			IGJK::init(json, game);
		}

		glm::vec2 get_center(glm::vec2 position, float sc) override
		{
			return (*direction_) * distance_* sc + position + offset_ * sc;
		}

		glm::vec2 support(glm::vec2 direction, glm::vec2 position, float sc) override
		{
			return get_center(position, sc);
		}

		glm::vec2 get_min(glm::vec2 position, float sc) override { return get_center(position, sc); }
		float get_width(float sc) override { return 1e-3f; }
		float get_height(float sc) override { return 1e-3f; }

		std::string get_id() { return "gjk-point"; }
	};
}