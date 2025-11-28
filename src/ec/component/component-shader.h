#pragma once
#include "ec/component/component.h"
#include <glad/glad.h>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "ec/component/component-template.h"
#include "ec/entity.h"

namespace Component {

	class Shader : public IComponent
	{
		GLuint id_;
		std::string vs_file_name_, fs_file_name_;

		void compile(const GLchar* vs_data, const GLchar* fs_data);
		void load();
	public:
		Shader()
			: id_(0)
		{}

		~Shader()
		{
			if (id_)
				glDeleteProgram(id_);
			id_ = 0;
		}

		void init(nlohmann::json json, Entity* game) override
		{
			vs_file_name_ = json["vs"].get<std::string>();
			fs_file_name_ = json["fs"].get<std::string>();
			load();
			
			// should probably remove this.
			this->use();
			auto projection = glm::ortho(0.0f, (GLfloat)game->get_component<Component::ValTemplate<float>>("width")->val, (GLfloat)game->get_component<Component::ValTemplate<float>>("height")->val, 0.0f, -1.0f, 1.0f);
			this->set_mat4("projection", projection);
		}

		void use();
		GLint get_attrib_location(const GLchar* attrib_name);

		// uniform sets
		void set_bool(const GLchar* name, GLboolean value);
		void set_int(const GLchar* name, GLint value);
		void set_float(const GLchar* name, GLfloat value);
		void set_vec2f(const GLchar* name, const glm::vec2& value);
		void set_vec2f(const GLchar* name, GLfloat x, GLfloat y);
		void set_vec3f(const GLchar* name, const glm::vec3& value);
		void set_vec3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z);
		void set_vec4f(const GLchar* name, const glm::vec4& value);
		void set_vec4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
		void set_mat2(const GLchar* name, const glm::mat2& matrix);
		void set_mat3(const GLchar* name, const glm::mat3& matrix);
		void set_mat4(const GLchar* name, const glm::mat4& matrix);
		std::string get_id() override { return "shader"; }
	};
}