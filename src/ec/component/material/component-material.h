#pragma once
#include "ec/component/component-texture.h"
#include "ec/component/component-shader.h"
#include "ec/component/component-template.h"
#include "ec/entity.h"
#include "templates.h"

/*
* Abstract holding class for texture shader combinations used for rendering
* @author David Wadsworth
*/
class IMaterial : public IComponent
{
protected:
	GLint tex_unit_;
public:
	Component::Texture* texture;
	Component::Shader* shader;
	GLuint id;

	IMaterial()
		: texture(texture), shader(shader), id(0), tex_unit_(-1)
	{}

		
	void init(nlohmann::json json, Entity* game) override
	{
		texture = game->get_nested_component<Component::Texture>(json["texture"]);
		shader = game->get_nested_component<Component::Shader>(json["shader"]);
		tex_unit_ = json["tex_unit"];
		id = game->get_component<Component::Int>("material_id")->val++;
	}

	virtual void compile() = 0; 
	virtual void bind() = 0;
};
