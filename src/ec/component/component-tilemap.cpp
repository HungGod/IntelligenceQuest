#include "ec/component/component-tilemap.h"
#include "ec/component/component-vec2.h"
#include "ec/component/component-quadtree.h"
#include "ec/component/component-tileset.h"

void Component::Tilemap::init(nlohmann::json json, Entity* game)
{
	width = json["width"];
	height = json["height"];

	auto json_map_arr = json["map"].get<std::vector<int>>();

	auto tileset = game->get_nested_component<Component::Tileset>(json["tileset"]);
	tile_size = tileset->tile_size;

	this->clear();
	for (auto i = 0; i < json_map_arr.size(); i += (int)width) {
		std::vector<int> layer;
		layer.insert(layer.end(), json_map_arr.begin() + i, json_map_arr.begin() + (i + (int)width));
		this->push_back(layer);
	}
}
