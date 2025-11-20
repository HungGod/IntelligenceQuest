#pragma once
#include "ec/component/src/component-src.h"
#include "logger.h"
#include "ec/entity.h"
#include "ec/component/material/component-material.h"

namespace Component
{
	class Box : public IComponent
	{
		std::vector<std::vector<ISrc*>> top_box_;
		std::vector<std::vector<ISrc*>> left_speech_arrow_;
		std::vector<std::vector<ISrc*>> right_speech_arrow_;
		std::vector<std::vector<ISrc*>> no_speech_arrow_; 
		glm::vec2 no_arrow_position;
		float tile_size;
		IMaterial* material_;

	public:
		
		struct BoxContainer
		{
			std::vector<std::vector<ISrc*>> srcs;
			float tile_size;
			IMaterial* material;
			glm::vec2 arrow_pos;
		};

		void init(nlohmann::json json, Entity* game)
		{
			float width = json["width"];
			float height = json["height"];
			tile_size = json["tile_size"].get<float>();
			Entity* spritesheet = game->get_nested_child(json["spritesheet"]);
			material_ = spritesheet->get_component<IMaterial>("material");

			no_arrow_position = glm::vec2(width / 2.0f * tile_size, height * tile_size);
			for (int i = 0; i < json["box"].size(); i+=width)
			{
				std::vector<ISrc*> srcs;
				for (int j = i; j < i + width; ++j)
				{
					if (json["box"][j].is_null())
						srcs.push_back(nullptr);
					else
						srcs.push_back(spritesheet->get_component<ISrc>(json["box"][j].get<std::string>()));	
				}
				top_box_.push_back(srcs);
			}

			for (int i = 0; i < json["left_speech_arrow"].size(); i+=width)
			{
				std::vector<ISrc*> srcs;

				for (int j = i; j < i + width; ++j)
				{
					if (json["left_speech_arrow"][j].is_null())
						srcs.push_back(nullptr);
					else
					{
						srcs.push_back(spritesheet->get_component<ISrc>(json["left_speech_arrow"][j].get<std::string>()));
					}
				}
				left_speech_arrow_.push_back(srcs);
			}

			for (auto i = 0; i < json["right_speech_arrow"].size(); i+=width)
			{
				std::vector<ISrc*> srcs;
				for (auto j = i; j < i + width; ++j)
				{
					if (json["right_speech_arrow"][j].is_null())
						srcs.push_back(nullptr);
					else
					{
						srcs.push_back(spritesheet->get_component<ISrc>(json["right_speech_arrow"][j].get<std::string>()));
					}
				}
				right_speech_arrow_.push_back(srcs);
			}

			for (auto i = 0; i < json["no_speech_arrow"].size();)
			{
				std::vector<ISrc*> srcs;
				for (auto j = i; j < width; ++j, ++i)
				{
					if (json["box"][j].is_null())
						srcs.push_back(nullptr);
					else
						srcs.push_back(spritesheet->get_component<ISrc>(json["no_speech_arrow"][j].get<std::string>()));
				}
				no_speech_arrow_.push_back(srcs);
			}
			
		}

		BoxContainer create_box(bool left, bool no_speech_arrow)
		{
			BoxContainer box_container;
			box_container.arrow_pos = no_arrow_position;
			box_container.tile_size = tile_size;
			box_container.material = material_;
			box_container.srcs.insert(box_container.srcs.end(), top_box_.begin(), top_box_.end());
		

			if (no_speech_arrow)
			{
				box_container.srcs.insert(box_container.srcs.end(), no_speech_arrow_.begin(), no_speech_arrow_.end());
			}
			else
			{
				if (left)
				{
					box_container.srcs.insert(box_container.srcs.end(), left_speech_arrow_.begin(), left_speech_arrow_.end());
				}
				else
					box_container.srcs.insert(box_container.srcs.end(), right_speech_arrow_.begin(), right_speech_arrow_.end());
			}

			return box_container;
		}

		std::string get_id() override { return "box"; }
	};
}