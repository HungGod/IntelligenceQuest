#pragma once
#include "ec/component/component.h"

class JsonTo
{
public:
	IComponent* component(nlohmann::json json, Entity* entity, Entity* game);
	std::vector<nlohmann::json> vector(nlohmann::json json);
};