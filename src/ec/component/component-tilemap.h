#pragma once
#include "ec/component/component.h"
#include <vector>

namespace Component
{
	class Tilemap : public IComponent, public std::vector<std::vector<int>>
	{
	public:
		float width, height;
		float tile_size;
		Tilemap()
			: width(0), height(0), tile_size(0)
		{}

		void init(nlohmann::json json, Entity* game);

		std::string get_id() override { return "tilemap"; }
	};
}
