#pragma once
#include "ec/component/component.h"
#include "ec/entity.h"

namespace Component
{
	class Intelligence : public IComponent, std::unordered_map<std::string, int>
	{
	public:
		Intelligence()
			: total(0)
		{}

		void init(nlohmann::json json, Entity* game) override
		{
			if (!json.is_null())
			{
				for (auto &json : json.get<nlohmann::json::object_t>())
				{
					int second = json.second;
					(*this)[json.first] = second;
					total += second;
				}
			}
		}

		nlohmann::json convert_to_json()
		{
			nlohmann::json json;
			for (auto pair : *this)
				json[pair.first] = pair.second;
			
			return json;
		}

		void add_intelligence(nlohmann::json json)
		{
			if (json.contains("id"))
			{
				std::string id = json["id"];
				if (this->count(id) == 0)
				{
					int iq = json["IQ"];
					(* this)[id] = iq;
					total += iq;
				}
			}
		}

		int total;

		std::string get_id() override { return "intelligence"; }
	};
}