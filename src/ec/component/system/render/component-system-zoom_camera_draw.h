#pragma once
#include "ec/component/system/component-system.h"
#include "ec/component/component-renderer.h"
#include "templates.h"
#include "ec/component/component-vec2.h"
#include "ec/component/src/component-src-flex.h"

namespace System
{
	class ZoomCameraDraw : public ISystem
	{
		Component::Renderer* renderer_;
		ISrc* src_;
		Component::Position* position_;
		IMaterial* material_;
		Component::Position* camera_position_;
		Component::Float* camera_zoom_;
		Component::Float* scale_;
	public:

		ZoomCameraDraw()
			: renderer_(nullptr), src_(nullptr), position_(nullptr), material_(nullptr), camera_position_(nullptr), camera_zoom_(nullptr), scale_(nullptr)
		{}

		void init(nlohmann::json json, Entity* game) override
		{
			src_ = game->get_nested_component<ISrc>(json["src"]);
			position_ = game->get_nested_component<Component::Position>(json["position"]);
			material_ = game->get_nested_component<IMaterial>(json["material"]);
			camera_position_ = game->get_nested_component<Component::Position>(json["camera_position"]);
			camera_zoom_ = game->get_nested_component<Component::Float>(json["camera_zoom"]);
			scale_ = game->get_nested_component<Component::Float>(json["scale"]);

			renderer_ = game->get_component<Component::Renderer>("renderer");
		}

		void execute() override
		{

			glm::vec2 screen_position = (*position_ - *camera_position_) * camera_zoom_->val;
			float screen_scale = scale_->val * camera_zoom_->val;

			src_->draw(screen_position, screen_scale, material_, renderer_);
		}
		std::string get_id() override { return "camera-draw"; }
	};
}
