#pragma once
#include "splay_array.h"
#include "ec/component/component.h"
#include "logger.h"
#include <string>
#include <json.hpp>

/*
storage class for components and other entities beneath it
can either id and retrieve components/children, or treat them as lists.

@author David Wadsworth
*/
class Entity
{
	std::string id_;
	SplayArray<IComponent> components_;
	SplayArray<Entity> children_;
public:
	Entity()
		: id_("entity")
	{}

	// add child without id
	Entity* push_back_child(Entity* entity)
	{
		return children_.push_back(entity);
	}

	// add child with id
	Entity* insert_child(Entity* entity, std::string id)
	{
		auto hashed_str = std::hash<std::string>{}(id);
		entity->id_ = id;
		return children_.insert(hashed_str, entity);
	}

	Entity* insert_child(std::string id)
	{
		auto entity = new Entity();
		auto hashed_str = std::hash<std::string>{}(id);
		entity->id_ = id;
		return children_.insert(hashed_str, entity);
	}

	Entity* insert_child(Entity* entity, std::size_t id)
	{
		entity->id_ = std::to_string(id);
		return children_.insert(id, entity);
	}

	Entity* insert_child(std::size_t id)
	{
		auto entity = new Entity();
		this->id_ = std::to_string(id);
		return children_.insert(id, entity);
	}
	
	// return child from when it was added
	Entity* get_child(std::size_t pos)
	{
		auto child = children_.get(pos);

		if (!child)
			Logger::error("could not find child, pos = " + std::to_string(pos), Logger::HIGH);

		return child;
	}

	Entity* get_nested_child(nlohmann::json json)
	{
		std::vector<nlohmann::json> path;
		if (json.is_array())
			path = json.get<std::vector<nlohmann::json>>();
		else
			path.push_back(json.get<std::string>());

		auto child = this;
		for (auto i = 0; i < path.size(); ++i)
		{
			if (path[i].is_array())
				child = get_nested_child(path[i]);
			else if (child->has_child(path[i]))
				child = child->get_child(path[i].get<std::string>());
			else
			{
				this->print();
				Logger::error("could not find nested child " + json.dump(1, '\t'), Logger::HIGH);
			}
		}
		return child;
	}

	// return child based on id
	Entity* get_child(std::string id)
	{
		auto hashed_str = std::hash<std::string>{}(id);
		auto child = children_.search(hashed_str);

		if (!child)
			Logger::error("could not find child, str id = " + id, Logger::HIGH);

		return child;
	}

	// returns all added children
	std::vector<Entity*> child_list()
	{
		return children_.get_list();
	}

	// checks for id match in splay array
	bool has_child(std::string id)
	{
		auto hashed_str = std::hash<std::string>{}(id);
		return children_.search(hashed_str);
	}

	// returns size of all added children
	std::size_t children_size()
	{
		return children_.size();
	}

	IComponent* push_back_component(IComponent* component)
	{
		components_.push_back(component);
		return component;
	}

	IComponent* insert_component(IComponent* component, std::string id)
	{
		component->set_name(id);
		auto hashed_str = std::hash<std::string>{}(id);
		components_.insert(hashed_str, component);
		return component;
	}

	IComponent* insert_component(IComponent* component, std::size_t id)
	{
		component->set_name(std::to_string(id));
		components_.insert(id, component);
		return component;
	}

	IComponent* get_raw_component(std::string id)
	{
		auto hashed_str = std::hash<std::string>{}(id);
		auto raw_comp = components_.search(hashed_str);

		return raw_comp;
	}

	IComponent* get_raw_component(std::size_t id)
	{
		return components_.search(id);
	}

	template <typename T>
	T* get_component(std::string id)
	{
		auto hashed_str = std::hash<std::string>{}(id);
		auto raw_comp = components_.search(hashed_str);

		if (!raw_comp)
		{
			print();
			Logger::error("could not find component, str id = " + id, Logger::HIGH);
		}

		auto casted_comp = dynamic_cast<T*>(raw_comp);

		if (!casted_comp)
			Logger::error("component not of casted type, str id = " + id, Logger::HIGH);

		return casted_comp;
	}

	template <typename T>
	T* get_nested_component(nlohmann::json json)
	{
		nlohmann::json path;
		std::string comp_id;
		if (json.is_null())
		{
			this->print();
			Logger::error("get_nested_component failed " + json.dump(1, '\t'), Logger::HIGH);
		}

		if (json.is_array())
		{
			for (auto i = 0; i < json.size() - 1; ++i) {
				path.push_back(json[i]);
			}
			comp_id = json.back();
		}
		else
			comp_id = json;

		auto child = get_nested_child(path);

		if (child->has_component(comp_id))
			return child->get_component<T>(comp_id);

		this->print();
		Logger::error("get_nested_component failed" + json.dump(1, '\t'), Logger::HIGH);
		return nullptr;
	}

	template <typename T>
	T* get_component(std::size_t pos)
	{
		auto component = components_.search(pos);

		if (!component)
			Logger::error("could not find component, pos = " + std::to_string(pos), Logger::HIGH);

		auto casted_component = dynamic_cast<T*>(component);

		if (!casted_component)
			Logger::error("component not of casted type, pos = " + std::to_string(pos), Logger::HIGH);

		return casted_component;
	}

	// checks if string id of component is located in component tree
	bool has_component(std::string id)
	{
		auto hashed_str = std::hash<std::string>{}(id);
		return components_.search(hashed_str);
	}

	// checks if id of component is located in component tree
	bool has_component(std::size_t pos)
	{
		return components_.search(pos);
	}

	std::vector<IComponent*> component_list()
	{
		return components_.get_list();
	}

	template <typename T> std::vector<T*> casted_component_list()
	{
		return components_.get_casted_list<T>();
	}

	std::size_t component_size()
	{
		return components_.size();
	}

	void print(int level = 0)
	{
		std::cout << std::string(level++ * 2, ' ') << this->to_string() << std::endl;
		std::string prev;
		int count = 1;
		for (auto c : component_list())
		{
			auto curr = c->to_string();
			if (curr == prev)
				count++;
			else if (!prev.empty())
			{
				if (count > 1)
				{
					std::cout << std::string(level * 2, ' ') << prev << " x" << count << std::endl;
					count = 1;
				}
				else
					std::cout << std::string(level * 2, ' ') << prev << std::endl;
			}
			prev = curr;
		}
		if (count > 1)
			std::cout << std::string(level * 2, ' ') << prev << " x" << count << std::endl;
		else
			std::cout << std::string(level * 2, ' ') << prev << std::endl;

		std::cout << std::endl;

		for (auto ch : child_list())
			ch->print(level);
	}

	std::string to_string()
	{
		return id_;
	}
};