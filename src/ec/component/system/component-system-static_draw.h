#pragma once
#include "ec/component/system/component-system.h"
#include "ec/component/component-renderer.h"
#include "rect.h"

namespace System {
	class StaticDraw : public ISystem
	{
		Component::Renderer* renderer_;
		ISrc* src_;
		Component::Position* position_;
		glm::vec4 dest_;
		IMaterial* material_;
		Component::Float* scale_;
	public:

		StaticDraw()
			: renderer_(nullptr), src_(nullptr), dest_(), material_(nullptr)
		{}

		void init(nlohmann::json json, Entity* game) override
		{
			src_ = game->get_nested_component<ISrc>(json["src"]);
			position_ = game->get_nested_component<Component::Position>(json["position"]);
			scale_ = game->get_nested_component<Component::Float>(json["scale"]);
			material_ = game->get_nested_component<IMaterial>(json["material"]);

			renderer_ = game->get_component<Component::Renderer>("renderer");
		}

		void execute() override
		{
			src_->draw(*position_, scale_->val, material_, renderer_);
		}

		std::string get_id() override { return "system-static_draw"; }
	};
}