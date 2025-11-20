#pragma once
#include "ec/entity.h"

class Game
{
	std::unique_ptr<Entity> game_;
public:
	Entity* init(std::string init_filepath);
};