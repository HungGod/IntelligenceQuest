#pragma once
#include "ec/component/component.h"
#include "rect.h"
#include "ec/component/component-vec2.h"
#include "ec/component/component-renderer.h"

class IBlock : public IComponent, public Rect
{
public:
	glm::vec2 offset;
	void init(nlohmann::json json, Entity* game) override
	{
		offset = glm::vec2(0);

		if (json.contains("offset"))
		{
			offset.x = json["offset"]["x"].get<float>();
			offset.y = json["offset"]["y"].get<float>();
		}
	}

	virtual void draw(glm::vec2 position, Component::Renderer* renderer) = 0;
};