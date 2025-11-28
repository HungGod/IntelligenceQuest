#pragma once
#include "ec/component/component-quadtree.h"
#include "command/command.h"
#include "ec/component/component-tilemap.h"
#include "ec/component/component-vec2.h"
#include "ec/component/component-template.h"

namespace Command
{
	class InitQuadTree : public ICommand
	{
		nlohmann::json quadtree_path_, tilemap_path_, scale_path_, position_path_;
	public:
		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
			quadtree_path_ = json["quadtree"];
			tilemap_path_ = json["tilemap"];
			position_path_ = json["position"];
			scale_path_ = json["scale"];
		}
		void execute(Entity* game, Component::Pathway* pathway) override
		{
			auto quadtree = game->get_nested_component<Component::QuadTree>(quadtree_path_);
			auto tilemap = game->get_nested_component<Component::Tilemap>(tilemap_path_);
			auto scale = game->get_nested_component<Component::ValTemplate<float>>(scale_path_);
			auto position = game->get_nested_component<Component::Vector2D>(position_path_);

			Rect map = { position->x, position->y, tilemap->width * tilemap->tile_size * scale->val, tilemap->height * tilemap->tile_size * scale->val };
			quadtree->init_rect(map);
			quadtree->all_clear();
		}
		std::string to_string() override { return "init_quadtree"; }
	};
}