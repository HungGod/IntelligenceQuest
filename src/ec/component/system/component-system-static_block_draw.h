#pragma once
#include "ec/component/system/component-system.h"
#include "ec/component/block/component-block.h"

namespace System
{
	class StaticBlockDraw : public ISystem
	{
		Component::Position* position_;
		Component::Renderer* renderer_;
	public:
		std::vector<IBlock*> blocks;

		void init(nlohmann::json json, Entity* game)
		{	
			position_ = game->get_nested_component<Component::Position>(json["position"]);
			renderer_ = game->get_component<Component::Renderer>("renderer");
			nlohmann::json blocks = json["blocks"];
			for (auto b : blocks)
				this->blocks.push_back(game->get_nested_component<IBlock>(b));
		}

		void execute() override
		{
			for (auto b : blocks)
				b->draw(*position_ + b->offset, renderer_);
		}

		std::string get_id() override { return "system-static_block_draw"; }
	};
}