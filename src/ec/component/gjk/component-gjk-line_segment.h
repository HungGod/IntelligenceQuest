#pragma once
#include "ec/component/gjk/component-gjk.h"
#include "ec/component/component-template.h"
#include "templates.h"
#include "ec/entity.h"

namespace GJK
{
	class LineSegment : public IGJK
	{
		glm::vec2 p1_{0.f}, p2_{0.f};
	public:

		LineSegment() = default;


        void init(nlohmann::json j, Entity* game) override
        {
            if (j.is_array() && j.size() >= 2) {
                p1_ = { j[0]["x"], j[0]["y"] };
                p2_ = { j[1]["x"], j[1]["y"] };
            }
            else if (j.contains("p1") && j.contains("p2")) {
                p1_ = { j["p1"]["x"], j["p1"]["y"] };
                p2_ = { j["p2"]["x"], j["p2"]["y"] };
            }
            else {
                Logger::error("LineSegment JSON needs p1/p2 or array: " + j.dump(1, '\t'), Logger::HIGH);
            }

            IGJK::init(j, game);
        }

        glm::vec2 get_center(glm::vec2 position, float sc) override
        {
            return position + (p1_ + p2_) * 0.5f * sc;
        }

        glm::vec2 support(glm::vec2 dir, glm::vec2 position, float sc) override
        {
            glm::vec2 w1 = position + p1_ * sc;
            glm::vec2 w2 = position + p2_ * sc;
            return (glm::dot(w1, dir) > glm::dot(w2, dir)) ? w1 : w2;
        }

        glm::vec2 get_min(glm::vec2 position, float sc) override
        {
            glm::vec2 w1 = p1_ * sc;
            glm::vec2 w2 = p2_ * sc;
            glm::vec2 mn = glm::min(w1, w2);
            return position + mn;
        }

        float get_width(float sc) override
        {
            return std::max(std::abs(p2_.x - p1_.x) * sc, 1e-3f);
        }

        float get_height(float sc) override
        {
            return std::max(std::abs(p2_.y - p1_.y) * sc, 1e-3f);
        }

		glm::vec2 resolve(IGJK* B, glm::vec2 A_pos, float A_sc, glm::vec2 B_pos, float B_sc) override
		{
			auto line = glm::normalize(p2_ - p1_);
			auto perp_bisector = glm::vec2(line.y, -line.x);

			auto sup = B->support(-perp_bisector, B_pos, B_sc) - this->support(perp_bisector, A_pos, A_sc);
			auto piercing_vec = glm::dot(sup, -perp_bisector) * perp_bisector;

			return piercing_vec;
		}

		std::string get_id() { return "gjk-line_segment"; }
	};
}
