#pragma once
#include "ec/component/system/component-system.h"
#include "ec/component/component-template.h"
#include "ec/component/component-vec2.h"
#include "templates.h"
#include <GLFW/glfw3.h>

namespace System
{
	class NPCMove : public ISystem
	{
		enum DIRECTIONS : uint8_t
		{
			UP,
			DOWN,
			LEFT,
			RIGHT,
			UP_RIGHT,
			UP_LEFT,
			DOWN_RIGHT,
			DOWN_LEFT
		};

		Component::Float* speed_, * delta_time_, * radius_;
		double end_time_;
		glm::vec2 center_;
		Component::Position* position_;
		Component::Direction* direction_;
	public:
		NPCMove()
			:  speed_(nullptr), delta_time_(nullptr), center_(), position_(nullptr), end_time_(0), direction_(nullptr), radius_(nullptr)
		{}

		void init(nlohmann::json json, Entity* game) override
		{
			position_ = game->get_nested_component<Component::Position>(json["position"]);
			direction_ = game->get_nested_component<Component::Direction>(json["direction"]);

			if (json.contains("radius"))
				radius_ = game->get_nested_component<Component::Float>(json["radius"]);
			else
				radius_ = nullptr;

			center_ = *position_;
			speed_ = game->get_nested_component<Component::Float>(json["speed"]);
			delta_time_ = game->get_component<Component::Float>("delta_time");
		}

		void execute() override
		{
			double curr_time = glfwGetTime();
			if (curr_time < end_time_)
			{
				position_->x += direction_->x * speed_->val * delta_time_->val;
				position_->y += direction_->y * speed_->val * delta_time_->val;
			}
			else
			{
				auto direction = std::rand() % 16;

				if (direction > 6 || radius_ == nullptr || radius_->val == 0)
				{
					direction_->x = 0;
					direction_->y = 0;

					end_time_ = curr_time + std::rand() % 2 * 0.5 + 0.5;
				}
				else
				{
					if (std::abs(position_->x - center_.x) > radius_->val || std::abs(position_->y - center_.y) > radius_->val)
					{
						glm::vec2 n_direction;

						n_direction.x =  center_.x - position_->x;
						n_direction.y =  center_.y - position_->y;
						n_direction = glm::normalize(n_direction);

						direction_->x = n_direction.x;
						direction_->y = n_direction.y;
					}
					else
					{
						switch (direction)
						{
						case UP:
							direction_->x = 0;
							direction_->y = -1;
							break;
						case DOWN:
							direction_->x = 0;
							direction_->y = 1;
							break;
						case RIGHT:
							direction_->x = 1;
							direction_->y = 0;
							break;
						case LEFT:
							direction_->x = -1;
							direction_->y = 0;
							break;
						case UP_LEFT:
							direction_->x = -1;
							direction_->y = -1;
							break;
						case UP_RIGHT:
							direction_->x = 1;
							direction_->y = -1;
							break;
						case DOWN_LEFT:
							direction_->x = -1;
							direction_->y = 1;
							break;
						case DOWN_RIGHT:
							direction_->x = 1;
							direction_->y = 1;
							break;
						default:
							break;
						}
					}
					end_time_ = curr_time + std::rand() % 2 * 0.3 + 0.3;
				}
				
			}
		}

		std::string get_id() override { return "system-npc_move"; }
	};
}