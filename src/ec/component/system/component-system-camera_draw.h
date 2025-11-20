#pragma once
#include "ec/component/system/component-system.h"
#include "ec/component/component-renderer.h"
#include "templates.h"
#include "ec/component/component-vec2.h"
#include "ec/component/src-flex/component-src-flex.h"

namespace System
{
	class CameraDraw : public ISystem
	{
		Component::Renderer* renderer_;
		ISrc* src_;
		Component::Position* position_;
		IMaterial* material_;
		Component::Position* camera_;
		Component::Float* scale_;
	public:

		CameraDraw()
			: renderer_(nullptr), src_(nullptr), position_(nullptr), material_(nullptr), camera_(nullptr), scale_(nullptr)
		{}

		void init(nlohmann::json json, Entity* game) override
		{
			src_ = game->get_nested_component<ISrc>(json["src_temp"]);
			position_ = game->get_nested_component<Component::Position>(json["position"]);
			material_ = game->get_nested_component<IMaterial>(json["material"]);
			camera_ = game->get_nested_component<Component::Position>(json["camera"]);
			scale_ = game->get_nested_component<Component::Float>(json["scale"]);

			renderer_ = game->get_component<Component::Renderer>("renderer");
		}

		void execute() override
		{
			src_->draw(*position_ - *camera_, scale_->val, material_, renderer_);
		}
		std::string get_id() override { return "camera-draw"; }
	};
}
