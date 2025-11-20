#pragma once
#include "ec/component/gjk/component-gjk.h"

namespace GJK
{
	class AABB : public IGJK
	{
		glm::vec2 offset_;
		float width_, height_;
	public:
		AABB()
			: offset_(), width_(0), height_(0)
		{}

		void init(nlohmann::json json, Entity* game) override
		{
			offset_.x = json["x"];
			offset_.y = json["y"];

			width_ = json["w"];
			height_ = json["h"];
		}

		glm::vec2 get_center(glm::vec2 position, float sc) override
		{
			return position + offset_ * sc + glm::vec2(width_ / 2.0f, height_ / 2.0f) * sc;
		}

		glm::vec2 get_min(glm::vec2 position, float sc) override
		{
			return position + offset_ * sc;
		}

		float get_width(float sc) override
		{
			return width_ * sc;
		}

		float get_height(float sc) override
		{
			return height_ * sc;
		}

		glm::vec2 support(glm::vec2 direction, glm::vec2 position, float sc) override
		{
			float sc_width = width_ * sc;
			float sc_height = height_ * sc;

			auto mag_max = glm::vec2((sc_width - direction.x) * (sc_width - direction.x), (sc_height - direction.y) * (sc_height - direction.y));
			auto mag_min = glm::vec2((-sc_width - direction.x) * (-sc_width - direction.x), (-sc_height - direction.y) * (-sc_height - direction.y));


			auto shortest_distance = mag_min.x + mag_min.y;
			glm::vec2 closest_vertex = get_min(position, sc);

			if (mag_max.x + mag_min.y < shortest_distance)
			{
				shortest_distance = mag_max.x + mag_min.y;
				closest_vertex = get_min(position, sc) + glm::vec2(sc_width, 0);
			}

			if (mag_max.x + mag_max.y < shortest_distance)
			{
				shortest_distance = mag_max.x + mag_min.y;
				closest_vertex = get_min(position, sc) + glm::vec2(sc_width, sc_height);
			}

			if (mag_min.x + mag_max.y < shortest_distance)
			{
				shortest_distance = mag_max.x + mag_min.y;
				closest_vertex = get_min(position, sc) + glm::vec2(0, sc_height);
			}

			return closest_vertex;
		}

		glm::vec2 resolve(IGJK* B, glm::vec2 A_pos, float A_sc, glm::vec2 B_pos, float B_sc) override
		{
			// world‐space centers
			glm::vec2 cA = get_center(A_pos, A_sc);
			glm::vec2 cB = B->get_center(B_pos, B_sc);
			glm::vec2 d = cA - cB;

			// half‐extents
			float hWA = 0.5f * get_width(A_sc);
			float hHA = 0.5f * get_height(A_sc);
			float hWB = 0.5f * B->get_width(B_sc);
			float hHB = 0.5f * B->get_height(B_sc);

			// overlap on each axis
			float overlapX = (hWA + hWB) - std::abs(d.x);
			float overlapY = (hHA + hHB) - std::abs(d.y);

			// if no overlap, no resolution
			if (overlapX <= 0.f || overlapY <= 0.f)
				return { 0.f, 0.f };

			// push out along smallest penetration axis
			if (overlapX < overlapY) {
				return { d.x < 0.f ? -overlapX : overlapX, 0.f };
			}
			else {
				return { 0.f, d.y < 0.f ? -overlapY : overlapY };
			}
		}

		
		std::string get_id() override { return "gjk-aabb"; }
	};
}