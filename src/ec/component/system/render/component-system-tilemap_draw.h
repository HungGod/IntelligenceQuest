#pragma once
#include "game.h"
#include "ec/component/system/component-system.h"
#include "ec/component/component-renderer.h"
#include "ec/component/component-tilemap.h"
#include "ec/component/component-tileset.h"

/*
executes systems stored in system array based on where the camera is located assuming a tiled array of stored systems

@author David Wadsworth
*/

namespace System {
	class TilemapDraw : public ISystem
	{
		Component::Renderer* renderer_;
		Component::Tileset* tileset_;
		Component::Tilemap* tilemap_;
		IMaterial* material_;

		Component::Vector2D* camera_pos_;
		Component::ValTemplate<float>* game_width_, * game_height_, *scale_;
	public:
		TilemapDraw()
			: renderer_(nullptr), tileset_(nullptr), tilemap_(nullptr), material_(nullptr),
				camera_pos_(nullptr), game_width_(nullptr), game_height_(nullptr), scale_(nullptr)
		{}

		void init(nlohmann::json json, Entity* game) override
		{
			tilemap_ = game->get_nested_component<Component::Tilemap>(json["tilemap"]);
			camera_pos_ = game->get_nested_component<Component::Vector2D>(json["camera"]);
			scale_ = game->get_nested_component<Component::ValTemplate<float>>(json["scale"]);

			renderer_ = game->get_component<Component::Renderer>("renderer");
			game_width_ = game->get_component<Component::ValTemplate<float>>("width");
			game_height_ = game->get_component<Component::ValTemplate<float>>("height");

			tileset_ = game->get_nested_component<Component::Tileset>(json["tileset"]);
			material_= game->get_nested_component<IMaterial>(json["material"]);
		}

		void execute() override final
		{
			auto scaled_tile_size = tilemap_->tile_size * scale_->val;

			// find the tiles that can fit on the screen based on right and left most parts of the screen
			auto left_norm = *camera_pos_ / scaled_tile_size - 1.0f;
			auto right_norm = left_norm;
			right_norm.x += game_width_->val / scaled_tile_size + 2.0f;
			right_norm.y += game_height_->val / scaled_tile_size + 2.0f;


			// draw the visible tiles
			for (auto j = static_cast<int>(left_norm.y); j < static_cast<int>(right_norm.y); ++j)
			{
				for (auto i = static_cast<int>(left_norm.x); i < static_cast<int>(right_norm.x); ++i)
				{
					if (i < 0 || i >= tilemap_->width || j < 0 || j >= tilemap_->size() || !(*tilemap_)[j][i])
						continue;

					const auto &tile = (*tileset_)[(*tilemap_)[j][i] - 1];

					glm::vec4 src{
						tile.x,
						tile.y,
						tile.x + tileset_->src_w,
						tile.y + tileset_->src_h
					};
					glm::vec4 dest{
						i * scaled_tile_size - camera_pos_->x,
						j * scaled_tile_size - camera_pos_->y,
						scaled_tile_size,
						scaled_tile_size
					};
					dest.z += dest.x;
					dest.w += dest.y;

					renderer_->draw(src, dest, material_);
				}
			}
			
		}

		std::string get_id() override { return "system-tilemap_draw"; }
	};
}
