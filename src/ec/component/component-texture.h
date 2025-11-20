#pragma once
#include <glad/glad.h>
#include <string>
#include "ec/component/component.h"

/*
image handling class. Responsible for loading and binding textures to tex slots.

@author David Wadsworth
*/
namespace Component
{
	class Texture : public IComponent
	{
		GLuint id_;
		void load(std::string tex_file_name);
	public:
		// image dimensions
		GLuint width, height;

		// texture format
		GLuint internal_format;
		GLuint image_format;

		// texture configuration
		GLuint wrap_s;
		GLuint wrap_t;
		GLuint filter_min;
		GLuint filter_mag;


		Texture()
			: width(0), height(0), internal_format(GL_RGBA), image_format(GL_RGBA), wrap_s(GL_REPEAT), wrap_t(GL_REPEAT), filter_min(GL_NEAREST),
			filter_mag(GL_NEAREST)
		{
			glGenTextures(1, &id_);
		}


		void init(nlohmann::json json, Entity* game) override
		{
			if (width == 0)
				load(json.get<std::string>());
		}

		~Texture()
		{
			glDeleteTextures(1, &id_);
		}

		void bind();
		std::string get_id() override { return "texture"; }
	};
}