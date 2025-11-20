#pragma once
#include <unordered_map>
#include <functional>
#include "ec/component/factory/component-factory.h"


namespace Factory
{
	struct CreateComponentMap : public IFactory, public std::unordered_map<std::string, std::function<IComponent*()>>
	{
		void create() override;

		std::string get_id() override { return "factory-create_component_map"; }
	};
}