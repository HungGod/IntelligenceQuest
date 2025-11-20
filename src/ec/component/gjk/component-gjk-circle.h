#pragma once
#include "ec/component/gjk/component-gjk.h"


namespace GJK
{
	class Circle : public IGJK
	{
		float radius_{0.f};
		glm::vec2 center_{0.f};
	public:

		void init(nlohmann::json json, Entity* game) override
		{
			radius_ = json["radius"];
			center_ = { json["x"], json["y"] };
		}

		glm::vec2 get_center(glm::vec2 position, float sc) override
		{
			return position + center_ * sc;
		}

		glm::vec2 support(glm::vec2 direction, glm::vec2 position, float sc) override
		{
			return get_center(position, sc) + radius_* sc * glm::normalize(direction);
		}

		glm::vec2 get_min(glm::vec2 position, float sc) override
		{
			return glm::vec2(position.x + center_.x * sc - radius_ * sc, position.y + center_.y * sc - radius_ * sc);
		}

		float get_width(float sc) override
		{
			return radius_ * 2 * sc;
		}

		float get_height(float sc) override
		{
			return get_width(sc);
		}

		
		glm::vec2 resolve(IGJK* B, glm::vec2 A_pos, float A_sc, glm::vec2 B_pos, float B_sc) override
		{
			auto perp_bisector = glm::normalize(B->get_center(B_pos, B_sc) - this->get_center(A_pos, A_sc));

			auto sup = B->support(-perp_bisector, B_pos, B_sc) - this->support(perp_bisector, A_pos, A_sc);
			auto piercing_vec = glm::dot(sup, -perp_bisector) * perp_bisector;

			return piercing_vec;
		}

		std::string get_id() override { return "gjk-circle"; }
	};
}