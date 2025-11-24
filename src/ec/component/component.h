#pragma once
#include <json.hpp>

class Entity;
struct IComponent {
	friend class Entity;
	IComponent()
		: name_()
	{}
	virtual void init(nlohmann::json json, Entity* game) {}
	std::string to_string() { return name_.empty() ? get_id() : name_ + ": " + get_id(); }
	bool has_name() { return !name_.empty(); }
	std::string get_name() { return name_; }
	virtual std::string get_id() = 0;
private:
	void set_name(std::string name) { name_ = name; }
	std::string name_;
};