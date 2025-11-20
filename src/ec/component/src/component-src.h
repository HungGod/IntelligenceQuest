#pragma once
#include "ec/component/component.h"
#include "rect.h"
#include <glm/glm.hpp>
#include "ec/component/component-renderer.h"

class ISrc : public IComponent
{
public:
	virtual void normalize(float img_width, float img_height) = 0;
	virtual glm::vec4 get_dest(glm::vec2 position, float scale) = 0;
	virtual glm::vec4 get_src() = 0;
	virtual float get_width() = 0;
	virtual float get_height() = 0;
	virtual void draw(glm::vec4 dest, IMaterial* material, Component::Renderer* renderer) { renderer->draw(get_src(), dest, material); };
	virtual void draw(glm::vec2 position, float scale, IMaterial* material, Component::Renderer* renderer) { draw(get_dest(position, scale), material, renderer); }
};
