#pragma once
#include "ec/component/src/component-src.h"
#include "ec/entity.h"
#include <cmath>

namespace Src
{
	class Transform : public ISrc
	{
		float rotation_angle_;
		bool flip_horizontal_, flip_vertical_;
	public:
		ISrc* src;

		Transform()
			: src(nullptr), rotation_angle_(0.0f), flip_horizontal_(false), flip_vertical_(false)
		{}

		void init(nlohmann::json json, Entity* game)
		{
			if (json.contains("path"))
				src = game->get_nested_component<ISrc>(json["path"]);
			else if (!json.is_null())
				src = game->get_nested_component<ISrc>(json);
			else
				src = nullptr;

			if (json.contains("rotation_angle"))
				rotation_angle_ = json["rotation_angle"];
			else
				rotation_angle_ = 0.0f;

			if (json.contains("flip_horizontal"))
				flip_horizontal_ = json["flip_horizontal"];
			else
				flip_horizontal_ = false;

			if (json.contains("flip_vertical"))
				flip_vertical_ = json["flip_vertical"];
			else
				flip_vertical_ = false;
		}

		void normalize(float img_width, float img_height) override final
		{
			if (!src)
				Logger::error("trying to normalize an unset src", Logger::HIGH);

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
			
			glm::vec4 transform_src = src->get_src();

			// Apply flipping
			if (flip_horizontal_) {
				std::swap(transform_src.x, transform_src.z);
			}
			if (flip_vertical_) {
				std::swap(transform_src.y, transform_src.w);
			}

			return transform_src;
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

		void draw(glm::vec4 dest, IMaterial* material, Component::Renderer* renderer)
		{
			if (!src) return;
			glm::vec4 src = this->get_src();

			if (rotation_angle_ != 0.0f) {
				// Calculate rotated corners
				std::array<glm::vec2, 4> rect_corners = calculate_rotated_corners(dest, rotation_angle_);
				// Pass rotated corners to the renderer
				renderer->draw(src, rect_corners, material);
			}
			else {
				// Pass unrotated destination rectangle to the renderer
				renderer->draw(src, dest, material);
			}
		}

		std::string get_id() override { return "src-transform"; }
	private:
		std::array<glm::vec2, 4> calculate_rotated_corners(glm::vec4 dest, float rotation_angle) {
			std::array<glm::vec2, 4> corners;

			// Calculate center of the rectangle
			glm::vec2 center = {
				(dest.x + dest.z) / 2.0f, // Center x
				(dest.y + dest.w) / 2.0f  // Center y
			};

			// Define corners of the rectangle
			glm::vec2 bottom_left = { dest.x, dest.w };
			glm::vec2 bottom_right = { dest.z, dest.w };
			glm::vec2 top_left = { dest.x, dest.y };
			glm::vec2 top_right = { dest.z, dest.y };

			// Convert angle to radians
			float angle_rad = glm::radians(rotation_angle);

			// Rotate each corner around the center
			corners[0] = rotate_point(bottom_left, center, angle_rad);  // Bottom-left
			corners[1] = rotate_point(bottom_right, center, angle_rad); // Bottom-right
			corners[2] = rotate_point(top_left, center, angle_rad);     // Top-left
			corners[3] = rotate_point(top_right, center, angle_rad);    // Top-right

			return corners;
		}

		// Helper function to rotate a point around a center
		glm::vec2 rotate_point(const glm::vec2& point, const glm::vec2& center, float angle_rad) {
			float s = std::sin(angle_rad);
			float c = std::cos(angle_rad);

			// Translate point to origin
			glm::vec2 translated = point - center;

			// Apply rotation
			glm::vec2 rotated = {
				translated.x * c - translated.y * s,
				translated.x * s + translated.y * c
			};

			// Translate back to the center
			return rotated + center;
		}

	};
}