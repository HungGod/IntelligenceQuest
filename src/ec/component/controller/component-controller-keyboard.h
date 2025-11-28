#pragma once
#include "ec/component/controller/component-controller.h"
#include "ec/component/component-template.h"
#include "ec/entity.h"

namespace Controller {
	class Keyboard : public IController
	{
		bool action_1_pressed_{ false },
			action_2_pressed_{ false },
			action_3_pressed_{ false },
			up_pressed_{ false },
			start_pressed_{ false };

		enum KeyID
		{
			left,
			right,
			down,
			up,
			action_1,
			action_2,
			action_3,
			start
		};

		std::array<int, 9> key_ids_{};
		Component::Template<std::array<bool, 1024>>* keys_;
	public:

		Keyboard() = default;

		void init(nlohmann::json json, Entity* game) override {
			keys_ = game->get_component<Component::Template<std::array<bool, 1024>>>("keys");

			key_ids_[left] = json["left"];
			key_ids_[right] = json["right"];
			key_ids_[down] = json["down"];
			key_ids_[up] = json["up"];
			key_ids_[action_1] = json["action_1"];
			key_ids_[action_2] = json["action_2"];
			key_ids_[action_3] = json["action_3"];
			key_ids_[start] = json["start"];
		}

		bool key_down_left() override
		{
			return (*keys_)[key_ids_[left]];
		}
		bool key_down_right() override
		{
			return (*keys_)[key_ids_[right]];
		}
		bool key_down_down() override
		{
			return (*keys_)[key_ids_[down]];
		}
		bool key_down_up() override
		{
			return (*keys_)[key_ids_[up]];
		}

		bool key_press_action_1() override
		{
			if ((*keys_)[key_ids_[action_1]])
			{
				if (!action_1_pressed_)
					action_1_pressed_ = true;
				else
					return false;
			}
			else
				action_1_pressed_ = false;

			return action_1_pressed_;
		}
		
		bool key_press_up() override
		{
			if ((*keys_)[key_ids_[up]])
			{
				if (!up_pressed_)
					up_pressed_ = true;
				else
					return false;
			}
			else
				up_pressed_ = false;

			return up_pressed_;
		}

		bool key_press_action_2() override
		{
			if ((*keys_)[key_ids_[action_2]])
			{
				if (!action_2_pressed_)
					action_2_pressed_ = true;
				else
					return false;
			}
			else
				action_2_pressed_ = false;

			return action_2_pressed_;
		}

		bool key_press_action_3() override
		{
			if ((*keys_)[key_ids_[action_3]])
			{
				if (!action_3_pressed_)
					action_3_pressed_ = true;
				else
					return false;
			}
			else
				action_3_pressed_ = false;

			return action_3_pressed_;
		}

		bool key_press_start() override
		{
			if ((*keys_)[key_ids_[start]])
			{
				if (!start_pressed_)
					start_pressed_ = true;
				else
					return false;
			}
			else
				start_pressed_ = false;

			return start_pressed_;
		}
		std::string get_id() override { return "controller-keyboard"; }
	};
}