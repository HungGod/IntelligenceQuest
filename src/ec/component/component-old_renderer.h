#pragma once
#include <glad/glad.h>
#include <vector>
#include <glm/vec4.hpp>
#include "ec/component/material/component-material.h"
#include "rect.h"

/*
Simple batch renderer for drawing sprites from the same image and shader

@author David Wadsworth
*/
namespace Component
{
	class OldRenderer : public IComponent
	{
		GLuint vbo_, vao_, att_size_, max_sprites_;
		std::vector<GLfloat> buffer_;
		IMaterial* current_mat_;
	public:
		void init(nlohmann::json json, Entity* game) override;

		OldRenderer();
		~OldRenderer() { release(); }

		void release();

		void begin();
		void draw(Rect src, Rect dest, IMaterial* mat);
		void flush();
		void end();
		std::string get_id() override { return "renderer"; }
	};
}
