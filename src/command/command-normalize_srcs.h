#pragma once
#include "command/command.h"
#include "ec/component/component-texture.h"
#include "ec/component/src/component-src.h"
#include "ec/entity.h"

namespace Command
{
	class NormalizeSrcs : public ICommand
	{
		nlohmann::json json_;
	public:

		void load(nlohmann::json json, Entity* game, Component::Pathway* pathway) override
		{
			json_ = json;
		}

		void execute(Entity* game, Component::Pathway* pathway) override
		{
			auto src_vector = game->get_nested_child(json_["path"])->casted_component_list<ISrc>();
			auto texture = game->get_nested_component<Component::Texture>(json_["texture"]);
			for (auto src : src_vector)
				src->normalize(texture->width, texture->height);
			
		}
		std::string to_string() override { return "normalize_srcs"; }
	};
}