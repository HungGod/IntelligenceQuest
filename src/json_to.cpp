#include "json_to.h"
#include "ec/component/factory/component-factory-create_component_map.h"
#include "ec/entity.h"
#include <glm/vec2.hpp>


IComponent* JsonTo::component(nlohmann::json json, Entity* entity, Entity* game)
{
	auto create_component_map = game->get_component<Factory::CreateComponentMap>("create_component_map");

	IComponent* component = nullptr;

	if (json.contains("points"))
	{
		nlohmann::json prev = nullptr;
		nlohmann::json line_segment;
		line_segment["type"] = "GJK::LineSegment";
		for (auto p : json["points"])
		{
			if (!prev.is_null())
			{

				glm::vec2 vec_prev = { prev["x"], prev["y"] };
				glm::vec2 vec_curr = { p["x"], p["y"] };

				glm::vec2 lowest = { vec_prev.x > vec_curr.x ? vec_curr.x : vec_prev.x, 
					vec_prev.y > vec_curr.y ? vec_curr.y : vec_prev.y };
				
				vec_prev -= lowest;
				vec_curr -= lowest;

				line_segment["parameters"]["p1"]["x"] = vec_prev.x;
				line_segment["parameters"]["p1"]["y"] = vec_prev.y;
				line_segment["parameters"]["p2"]["x"] = vec_curr.x;
				line_segment["parameters"]["p2"]["y"] = vec_curr.y;
				line_segment["parameters"]["pos"]["x"] = lowest.x;
				line_segment["parameters"]["pos"]["y"] = lowest.y;
				JsonTo::component(line_segment, entity, game);
			}
			prev = p;
		}
		return component;
	}

	if (!json.contains("type"))
	{
		// load texturepacker gui spritesheet format
		if (json.contains("frame")) 
		{
			if (entity->has_component(json["filename"].get<std::string>()))
				component = entity->get_raw_component(json["filename"].get<std::string>());
			else
				component = entity->insert_component((* create_component_map)["Src"](), json["filename"].get<std::string>());

			component->init(json, game);
		}
		else if (json.contains("name"))
		{
			component = entity->get_raw_component(json["name"].get<std::string>());
			if (component == nullptr)
				Logger::error("no component found of name: " + json["name"].get<std::string>(), Logger::HIGH);
			component->init(json["parameters"], game);
		}
		else
		{
			Logger::error("Could not find component from json " + json.dump(1, '\t'), Logger::LOW);
			return nullptr;
		}
	}
	else
	{
		std::string type = json["type"];
		/*if (json["type"].get<std::string>() == "GJK::AABB")
		{
			if (!json["parameters"].contains("state") || json["parameters"]["state"] == "both" || json["paramters"]["state"] == "physical")
			{
				float x = json["parameters"]["x"];
				float y = json["parameters"]["y"];
				float w = json["parameters"]["w"];
				float h = json["parameters"]["h"];

				nlohmann::json p1;
				p1["x"] = x;
				p1["y"] = y;
				nlohmann::json p2;
				p2["x"] = x + w;
				p2["y"] = y;
				nlohmann::json p3;
				p3["x"] = x + w;
				p3["y"] = y + h;
				nlohmann::json p4;
				p4["x"] = x;
				p4["y"] = y + h;


				nlohmann::json line_segment;
				line_segment["type"] = "GJK::LineSegment";
				line_segment["parameters"]["p1"] = p1;
				line_segment["parameters"]["p2"] = p2;

				JsonTo::component(line_segment, entity, game);
				line_segment["parameters"]["p1"] = p2;
				line_segment["parameters"]["p2"] = p3;

				JsonTo::component(line_segment, entity, game);
				line_segment["parameters"]["p1"] = p3;
				line_segment["parameters"]["p2"] = p4;

				JsonTo::component(line_segment, entity, game);
				line_segment["parameters"]["p1"] = p4;
				line_segment["parameters"]["p2"] = p1;

				JsonTo::component(line_segment, entity, game);

				if (!json["parameters"].contains("state") || json["parameters"]["state"] == "physical")
					return component;

				json["parameters"]["state"] = "actionable";
			}
		}*/

		// load in bitmap glyphs generated from BM Font
		if (json["type"].get<std::string>() == "BMFont")
		{
			for (auto params : json["parameters"])
			{
				nlohmann::json bitmap_glyph;
				bitmap_glyph["type"] = "BitmapGlyph";
				bitmap_glyph["name"] = params["id"];
				bitmap_glyph["parameters"] = params;
				JsonTo::component(bitmap_glyph, entity, game);
			}
			return component;
		}

		if (json.contains("name") && !json["name"].is_null())
		{
			if (json["name"].is_string()) // I could make "name" a union and reduce 20 lines of code but I couldn't be bothered. 
			{
				std::string name = json["name"];
				component = entity->get_raw_component(name);
				if (!component)
				{
					try {
						component = entity->insert_component((*create_component_map)[type](), name);
					}
					catch (std::exception e)
					{
						Logger::error("Could not create component id = " + type + "\n" + json.dump(1, '\t'), Logger::HIGH);
					}
				}
			}
			else
			{
				int name = json["name"]; 
				component = entity->get_raw_component(name);
				if (!component)
				{
					try {
						component = entity->insert_component((*create_component_map)[type](), name);
					}
					catch (std::exception e)
					{
						Logger::error("Could not create component id = " + type + "\n" + json.dump(1, '\t'), Logger::HIGH);
					}
				}
			}
			
		}
		else
		{
			try {
				component = entity->push_back_component((*create_component_map)[type]());
			}
			catch (std::exception e)
			{
				Logger::error("Could not create component id = " + type + "\n" + json.dump(1, '\t'), Logger::HIGH);
			}
		}

		if (json.contains("parameters"))
		{
			component->init(json["parameters"], game);
		}
		else if (json.contains("objectgroup"))
			component->init(json, game);
		else
			component->init(nlohmann::json(), game);
	}

	return component;
}

std::vector<nlohmann::json> JsonTo::vector(nlohmann::json json)
{
	std::vector<nlohmann::json> json_arr;
	if (json.is_array())
		json_arr = json.get<std::vector<nlohmann::json>>();
	else
		json_arr.push_back(json);

	return json_arr;
}