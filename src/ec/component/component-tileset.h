#pragma once
#include "ec/component/component.h"
#include "ec/entity.h"
#include "ec/component/component-texture.h"
#include <glm/glm.hpp>

namespace Component
{
	struct Tileset : public IComponent, public std::vector<glm::vec2>
	{
		float tile_size;
		float src_w, src_h;

		Tileset()
			: tile_size(0), src_w(0), src_h(0)
		{}

		void init(nlohmann::json json, Entity* game) override
		{
			auto texture = game->get_nested_component<Component::Texture>(json["texture"]);
			tile_size = json["tile_size"];
			float margin = json["margin"];
			float spacing = json["spacing"];

			auto height = (float)texture->height;
			auto width = (float)texture->width;
			src_w = tile_size / width;
			src_h = tile_size / height;
			this->clear();
			for (auto i = margin; i < height; i += (spacing+tile_size))
				for (auto j = margin; j < width; j += (spacing+tile_size))
					this->push_back( {j / width, i / height} );
		}

		std::string get_id() override { return "tileset"; }
	};
}