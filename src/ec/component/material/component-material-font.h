#pragma once
#include "ec/component/material/component-material.h"

namespace Material
{
	class Font : public IMaterial
	{
		glm::vec3 color_;
	public:
		Font()
			: color_(0)
		{}

		void init(nlohmann::json json, Entity* game) override {
			IMaterial::init(json, game);
			if (json.contains("color"))
			{
				color_.x = json["color"]["x"];
				color_.y = json["color"]["y"];
				color_.z = json["color"]["z"];
			}
		}

		void compile() override {
			// set up tex unit in shader program
			shader->use();
			shader->set_int("image", tex_unit_);
			shader->set_vec3f("spriteColor", color_);
		}

		void bind() override {
			// activate defined tex unit and bind appropriate texture onto it
			glActiveTexture(GL_TEXTURE0 + tex_unit_);
			texture->bind();
		}

		std::string get_id() override { return "material-font"; }
	};
}