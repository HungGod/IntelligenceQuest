#pragma once
#include "ec/component/component.h"
#include "rect.h"
#include "logger.h"
#include "ec/component/component-template.h"
#include <sstream>
#include "ec/entity.h"
#include "ec/component/component-collider.h"

namespace Component
{
	constexpr auto MAX_INDEX = 4;
	class QuadTree : public IComponent
	{
		Rect rect_;
		std::vector<Component::Collider*> objects_;
		std::size_t max_objects_;
		std::array<QuadTree*, MAX_INDEX> trees_;
		Component::Template<std::vector<Component::QuadTree*>>* all_trees_;

		enum quadrants: unsigned
		{
			top_left = 0,
			top_right = 1,
			bot_right = 2,
			bot_left = 3
		};

		std::vector<unsigned> index(Rect obj)
		{
			float vertical_midpoint = rect_.x + (rect_.w / 2.0f);
			float horizontal_midpoint = rect_.y + (rect_.h / 2.0f);

			float x = obj.x;
			float y = obj.y;
			float w = obj.w;
			float h = obj.h;

			bool top_quadrant = y < horizontal_midpoint;
			bool bottom_quadrant = y + h > horizontal_midpoint;

			bool left_quadrant = x < vertical_midpoint;
			bool right_quadrant = x + w > vertical_midpoint;

			std::vector<unsigned> index;

			if (left_quadrant) {
				if (top_quadrant) index.push_back(top_left) ;
				if (bottom_quadrant) index.push_back(bot_left);
			}
			if (right_quadrant) {
				if (top_quadrant) index.push_back(top_right);
				if (bottom_quadrant) index.push_back(bot_right);
			}
			return index;
		}

		void split()
		{
			if (index_->val + MAX_INDEX > all_trees_->size())
			{
				print_all_trees();
				print(std::cout);
				Logger::error("QuadTree exceeds max allocated trees", Logger::SEVERITY::HIGH);
			}

			auto x = rect_.x;
			auto y = rect_.y;
			auto sub_w = rect_.w / 2.0f;
			auto sub_h = rect_.h / 2.0f;

			// inits 4 quad trees at index and adds entities to the prior tree's children
			trees_[top_left] = all_trees_->at(index_->val++)->init_rect(Rect{ x, y, sub_w, sub_h });
			trees_[top_right] = all_trees_->at(index_->val++)->init_rect(Rect{ x + sub_w, y, sub_w, sub_h });
			trees_[bot_right] = all_trees_->at(index_->val++)->init_rect(Rect{ x + sub_w, y + sub_h, sub_w, sub_h });
			trees_[bot_left] = all_trees_->at(index_->val++)->init_rect(Rect{ x, y + sub_h, sub_w, sub_h });
		
		}

		void retrieve(Rect col, std::vector<Component::Collider*>& retrieved_objs)
		{
			for (auto &obj : objects_)
				retrieved_objs.push_back(obj);

			if (trees_[top_left] != nullptr)
				for (auto in : index(col))
				{
					auto in_tree = trees_[in];
					in_tree->retrieve(col, retrieved_objs);
				}
		}

		void insert(Component::Collider* obj)
		{
			if (objects_.size() >= max_objects_) {
				split();
				std::vector<Component::Collider*> multi_quadrant_objects{};
				for (auto i = 0u; i < objects_.size(); i++)
				{
					auto col = objects_[i];

					auto in = index(col->get_rect());

					if (in.size() == 1) trees_[in.front()]->insert(col);
					else multi_quadrant_objects.push_back(col);
				}
				objects_ = multi_quadrant_objects;
			}

			if (trees_[top_left] != nullptr)
			{
				auto in = index(obj->get_rect());
				if (in.size() == 1) 
				{
					trees_[in.front()]->insert(obj);
					return;
				}
			}

			objects_.push_back(obj);
		}

		void clear()
		{
			objects_.clear();
			trees_ = {nullptr, nullptr, nullptr, nullptr};
		}
	public:
		Component::ValTemplate<int>* index_;

		QuadTree()
			: index_(nullptr), all_trees_(nullptr), max_objects_(0), rect_{0}, trees_{nullptr, nullptr, nullptr, nullptr}
		{}

		QuadTree(Component::Template<std::vector<Component::QuadTree*>>* all_trees, Component::ValTemplate<int>* index, std::size_t max_objects)
			: all_trees_(all_trees), max_objects_(max_objects), index_(index), rect_{0}, trees_{nullptr, nullptr, nullptr, nullptr}
		{
			objects_.reserve(max_objects);
		}

		QuadTree* init_rect(Rect rect)
		{
			clear();
			rect_ = rect;
			return this;
		}

		void init(nlohmann::json json, Entity* game) override
		{
			if (all_trees_ != nullptr)
			{
				Logger::error("QuadTree already initialized", Logger::SEVERITY::HIGH);
				return;
			}

			auto root_entity = game->get_nested_child(json["path"]);
			max_objects_ = json["max_objects"];
			int max_trees = json["max_trees"];

			all_trees_ = static_cast<Component::Template<std::vector<Component::QuadTree*>>*>(root_entity->push_back_component(new Component::Template<std::vector<Component::QuadTree*>>()));
			all_trees_->reserve(max_trees);
			index_ = static_cast<Component::ValTemplate<int>*>(root_entity->push_back_component(new Component::ValTemplate<int>()));
			index_->val = 0;
			objects_.reserve(max_objects_);

			for (auto i = 0; i < max_trees; ++i)
				all_trees_->push_back(static_cast<QuadTree*>(root_entity->push_back_component(new Component::QuadTree(all_trees_, index_, max_objects_))));
		}

		void print_all_trees()
		{
			std::cout << std::endl;
			int object_count = this->objects_.size();
			for (auto tree : *all_trees_)
				object_count += tree->objects_.size();
			std::cout << "object_count: " << object_count << std::endl;
		}

		void print(std::ostream& os, int level = 0, std::string prefix = "r")
		{
			os << std::endl;
			auto t_rect = this->rect_;
			os << std::string(level, '\t') << prefix<< ": (" << t_rect.x << ", " << t_rect.y << ", "
				<< t_rect.w << ", " << t_rect.h << ")" << std::endl;
			for (auto obj : objects_)
				os << std::string(level, '\t') << prefix << " -> o: (" << obj->get_rect().x << ", " << obj->get_rect().y << ", "
					<< obj->get_rect().w << ", " << obj->get_rect().h << ")" << std::endl;
			if (trees_[top_left] != nullptr)
			{
				trees_[top_left]->print(os, level + 1, "tl");
				trees_[top_right]->print(os, level + 1, "tr");
				trees_[bot_right]->print(os, level + 1, "br");
				trees_[bot_left]->print(os, level + 1, "bl");
			}

		}

		void add(Component::Collider* col)
		{
			insert(col);
		}

		void add(std::vector<Component::Collider*>& colliders)
		{
			for (Component::Collider* col : colliders)
				add(col);
		}

		void all_clear()
		{
			for (int i = 0; i < index_->val; i++)
				all_trees_->at(i)->clear();
			clear();
			index_->val = 0;
		}

		std::vector<Component::Collider*> retrieve(Rect rect)
		{
			std::vector<Component::Collider*> retrieved;
			retrieve(rect, retrieved);
			return retrieved;
		}

		std::vector<Component::Collider*> retrieve(Component::Collider* col)
		{
			std::vector<Component::Collider*> retrieved;
			retrieve(col->get_rect(), retrieved);
			return retrieved;
		}

		int size()
		{
			return objects_.size();
		}
		
		std::string get_id() override { return "quadtree"; }
	};
}