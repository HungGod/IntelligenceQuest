#pragma once
#include "ec/component/system/component-system.h"
#include "ec/component/component-renderer.h"
#include "ec/component/component-vec2.h"
#include "ec/component/src/component-src-flex.h"

namespace System
{
	class CameraDraw : public ISystem
	{
		Component::Renderer* renderer_;
		ISrc* src_;
		Component::Vector2D* position_;
		IMaterial* material_;
		Component::Vector2D* camera_;
		Component::ValTemplate<float>* scale_;
	public:

		CameraDraw()
			: renderer_(nullptr), src_(nullptr), position_(nullptr), material_(nullptr), camera_(nullptr), scale_(nullptr)
		{}

		void init(nlohmann::json json, Entity* game) override
		{
			src_ = game->get_nested_component<ISrc>(json["src_temp"]);
			position_ = game->get_nested_component<Component::Vector2D>(json["position"]);
			material_ = game->get_nested_component<IMaterial>(json["material"]);
			camera_ = game->get_nested_component<Component::Vector2D>(json["camera"]);
			scale_ = game->get_nested_component<Component::ValTemplate<float>>(json["scale"]);

			renderer_ = game->get_component<Component::Renderer>("renderer");
		}

		void execute() override
		{
			src_->draw(*position_ - *camera_, scale_->val, material_, renderer_, 1);
		}
		std::string get_id() override { return "camera-draw"; }
	};
}
