#pragma once
#include "ec/component/block/component-block.h"

namespace Block
{
	class Image : public IBlock
	{
	public:

		void init(nlohmann::json json, Entity* game) override
		{
			// todo 
		}

		void draw(glm::vec2 position, Component::Renderer* renderer) override
		{
			// todo
		}

		std::string get_id() override { return "block-image"; }
	};
}