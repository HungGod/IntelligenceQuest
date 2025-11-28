#pragma once
#include "ec/component/system/component-system.h"
#include "ec/component/block/component-block.h"

namespace System
{
	class CameraBlockDraw : public ISystem
	{
		Component::Vector2D* position_;
		Component::Renderer* renderer_;
		Component::Vector2D* camera_;
	public:
		std::vector<IBlock*> blocks;

		void init(nlohmann::json json, Entity* game)
		{
			position_ = game->get_nested_component<Component::Vector2D>(json["position"]);
			renderer_ = game->get_component<Component::Renderer>("renderer");
			camera_ = game->get_nested_component<Component::Vector2D>(json["camera"]);
			nlohmann::json blocks = json["blocks"];
			for (auto b : blocks)
				this->blocks.push_back(game->get_nested_component<IBlock>(b));
		}

		void execute() override
		{
			for (auto b : blocks)
				b->draw(*position_ - *camera_ + b->offset, renderer_);
		}
		std::string get_id() override { return "system-camera_block_draw"; }
	};
}