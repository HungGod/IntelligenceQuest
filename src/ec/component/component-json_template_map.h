#pragma once
#include "ec/component/component.h"
#include "json_to.h"
#include <fstream>
#include <sstream>
#include "logger.h"
#include "json_template.h"

namespace Component
{
	class JsonTemplateMap: public IComponent, public std::unordered_map<std::string, std::unique_ptr<JsonTemplate>>
	{
	public:
		
		void init(nlohmann::json json, Entity* game)
		{
			JsonTo json_to;
			for (auto &temp : json_to.vector(json))
			{
				std::stringstream template_stream;
				std::string filename = temp;
				try
				{
					// open files
					std::ifstream indx_file(filename);

					// read into temp string streams
					template_stream << indx_file.rdbuf();

					// close file streams
					indx_file.close();
				}
				catch (std::exception e) {
					Logger::error("Failed to read template file! path = " + temp, Logger::HIGH);
				}
				auto json_template = new JsonTemplate();
				json_template->create_template(template_stream.str());

				(*this)[filename] = std::unique_ptr<JsonTemplate>(json_template);
			}
		}

		std::string get_id() override { return "template_map"; }
	};
}