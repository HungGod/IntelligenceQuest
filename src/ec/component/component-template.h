#pragma once
#include "ec/component/component.h"
#include <unordered_map>
#include "json.hpp"

namespace Component {
	template <typename T>
	struct ValTemplate : public IComponent
	{
		void init(nlohmann::json json, Entity* game) override
		{
			if (!json.is_null())
				val = json.get<T>();
		}

		T val;
		std::string get_id() override { return "val_template"; }
	};

	template <typename T>
	struct Template : public IComponent, public T {
		using T::T;
		std::string get_id() override { return "template"; }
	};
}