#pragma once
#include "ec/component/component.h"

class JsonTo
{
public:
	IComponent* component(nlohmann::json json, Entity* entity , Entity* game = nullptr);
	std::vector<nlohmann::json> vector(nlohmann::json json);
};