#pragma once
#include "ec/component/material/component-material.h"

namespace Material
{
	class Sprite : public IMaterial
	{
	public:

		void compile() override {
			// set up tex unit in shader program
			shader->use();
			shader->set_int("image", tex_unit_);
		}

		void bind() override {
			// activate defined tex unit and bind appropriate texture onto it
			glActiveTexture(GL_TEXTURE0 + tex_unit_);
			texture->bind();
		}

		std::string get_id() override { return "material-sprite"; }
	};
}