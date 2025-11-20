#pragma once
#include "ec/component/system/component-system.h"
#include "ec/component/controller/component-controller.h"
#include "ec/component/component-pathway.h"


namespace System
{
	class ExecuteJson : public ISystem
	{
		IController* controller_;
		Component::Pathway* pathway_;
		nlohmann::json commands_;
	public:

		ExecuteJson()
			: controller_(nullptr), pathway_(nullptr)
		{}

		void init(nlohmann::json json, Entity* game) override
		{
			controller_ = game->get_nested_component<IController>(json["controller"]);
			pathway_ = game->get_component<Component::Pathway>("pathway");
			commands_ = json["commands"];
		}

		void execute() override
		{
			if (controller_->key_press_action_2())
				pathway_->message(commands_);
		}

		std::string get_id() override { return "save_state"; }
	};
}