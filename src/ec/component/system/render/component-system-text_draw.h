#pragma once
#include "ec/component/system/component-system.h"
#include "ec/entity.h"
#include "ec/component/component-renderer.h"
#include "text.h"

/*
* TextDraw (Deprecated): draws text on the screen without regards to camera position
*/

namespace System
{
	class TextDraw : public ISystem
	{
	protected:

		Component::Renderer* renderer_;
		IMaterial* font_material_;
		Text text_;
	public:
		TextDraw()
			: renderer_(nullptr), font_material_(nullptr)
		{}

		void init(nlohmann::json json, Entity* game) override
		{
			// get font information
			Entity* font = game->get_nested_child(json["font"]);
			font_material_ = font->get_component<IMaterial>("material");


			renderer_ = game->get_component<Component::Renderer>("renderer");
			float scale = json["scale"];
			glm::vec2 position{};
			position.x = json["position"]["x"].get<float>();
			position.y = json["position"]["y"].get<float>();

			std::string message = json["message"];

			text_.init(font, scale);
			text_.write(message, position);
		}

		void execute() override
		{
			for (auto i = 0; i < text_.glyphs.size(); ++i)
				text_.glyphs[i]->draw(text_.dests[i], font_material_, renderer_);
		}

		std::string get_id() override {
			return "system-text_draw";
		}
	};
}
