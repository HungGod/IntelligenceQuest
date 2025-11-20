#pragma once
#include "ec/component/component.h"
#include <unordered_map>
#include "glyph.h"
#include "ec/component/component-texture.h"
#include "ec/entity.h"

namespace Component
{
	class FontMap : public IComponent, public std::unordered_map<int, Glyph>
	{
	public:
		void init(nlohmann::json json, Entity* game) override
		{
			auto texture = game->get_nested_component<Component::Texture>(json["texture"]);
			for (auto js : json["char_srcs"])
			{
				Glyph glyph{
					Rect {
						js["x"].get<float>(),
						js["y"].get<float>(),
						js["width"].get<float>(),
						js["height"].get<float>()
					},
					js["xadvance"].get<int>(),
					glm::vec2{
						js["offsetx"].get<float>(),
						js["offsety"].get<float>()
					}
				};

				(*this)[js["id"].get<int>()] = glyph;
			}
		}

		std::string get_id() override { return "font_map"; }
	};
}