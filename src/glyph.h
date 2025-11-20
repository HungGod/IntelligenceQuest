#pragma once
#include "rect.h"
#include <glm/glm.hpp>

struct Glyph
{
	Rect src;
	int x_advance;
	glm::vec2 offset;
};