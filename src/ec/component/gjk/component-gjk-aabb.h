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
		// Support function returns the farthest point in the given direction
		// For an AABB, this is simply the corner that maximizes dot(direction, corner)
		float sc_width = width_ * sc;
		float sc_height = height_ * sc;
		
		glm::vec2 min = get_min(position, sc);
		
		// Choose corner based on direction signs
		glm::vec2 corner = min;
		if (direction.x > 0.f) corner.x += sc_width;
		if (direction.y > 0.f) corner.y += sc_height;
		
		return corner;
	}

	glm::vec2 resolve(IGJK* B, glm::vec2 A_pos, float A_sc, glm::vec2 B_pos, float B_sc) override
	{
		// Get centers of both shapes
		glm::vec2 centerA = get_center(A_pos, A_sc);
		glm::vec2 centerB = B->get_center(B_pos, B_sc);
		
		// Direction from A center to B center
		glm::vec2 centerDir = centerB - centerA;
		
		// For AABB, we want to resolve along the axis with the smallest component
		// This ensures we push perpendicular to an edge
		glm::vec2 resolutionDir{0.f, 0.f};
		
		if (std::abs(centerDir.x) > std::abs(centerDir.y)) {
			// Push along X axis (perpendicular to vertical edge)
			resolutionDir = glm::vec2((centerDir.x > 0.f) ? 1.f : -1.f, 0.f);
		} else {
			// Push along Y axis (perpendicular to horizontal edge)
			resolutionDir = glm::vec2(0.f, (centerDir.y > 0.f) ? 1.f : -1.f);
		}
		
		// Handle case where centers are very close (B center inside A)
		if (glm::length2(centerDir) < GJK_EPSILON) {
			// Use a default direction (prefer X axis)
			resolutionDir = glm::vec2(1.f, 0.f);
		}
		
		// Use support points to get accurate penetration depth
		// This works correctly even when B's center is inside A
		glm::vec2 As = support(resolutionDir, A_pos, A_sc);
		glm::vec2 Bs = B->support(-resolutionDir, B_pos, B_sc);
		float pen = glm::dot(As - Bs, resolutionDir);
		
		// Ensure penetration is positive (should always be if colliding)
		if (pen < 0.f) pen = -pen;
		
		return resolutionDir * pen;
	}

		
		std::string get_id() override { return "gjk-aabb"; }
	};
}