#pragma once
#include "ec/component/component.h"
#include "rect.h"

namespace Component
{
	class Rectangle : public IComponent, public Rect
	{
	public:

		void init(nlohmann::json json, Entity* game)
		{
			this->x = json["x"];
			this->y = json["y"];
			this->w = json["w"];
			this->h = json["h"];
		}

	};
}