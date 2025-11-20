#pragma once
#include "ec/component/component-animation.h"
#include "ec/component/component-animation_map.h"
#include "ec/component/block/component-block.h"

namespace Block
{
	class Animation : public IBlock
	{
		Component::Animation* animation_;
		Component::AnimationMap* animation_map_;
	public:

		void init(nlohmann::json json, Entity* game) override
		{
			// todo 
		}

		void draw(glm::vec2 position, Component::Renderer* renderer) override
		{
			// todo
		}

		std::string get_id() override { return "block-animation"; }
	};
}