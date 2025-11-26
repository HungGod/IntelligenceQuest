#pragma once
#include "ec/component/component.h"
#include "rect.h"
#include "logger.h"
#include "ec/component/component-template.h"
#include "templates.h"
#include <sstream>
#include "ec/entity.h"
#include "ec/component/component-collider.h"

namespace Component
{
	constexpr auto MAX_INDEX = 4;
	class QuadTree : public IComponent
	{
		Rect rect_;
		std::array<QuadTree*, MAX_INDEX> trees_;
		std::vector<Component::Collider*> objects_;
		std::size_t max_objects_;
		bool split_;
		Component::QuadTreeVector* all_trees_;

		enum quadrants
		{
			top_left,
			top_right,
			bot_right,
			bot_left
		};

		unsigned index(Rect obj)
		{
			auto vertical_midpoint = rect_.x + (rect_.w / 2.0f);
			auto horizontal_midpoint = rect_.y + (rect_.h / 2.0f);

			auto x = obj.x;
			auto y = obj.y;
			auto w = obj.w;
			auto h = obj.h;

			auto top_quadrant = y < horizontal_midpoint;
			auto bottom_quadrant = y + h > horizontal_midpoint;

			auto left_quadrant = x < vertical_midpoint;
			auto right_quadrant = x + w > vertical_midpoint;


			auto i = 0u;
			auto index = 0u;

			if (left_quadrant) {
				if (top_quadrant) index += top_left << (i++ << 1u);
				if (bottom_quadrant) index += bot_left << (i++ << 1u);
			}
			if (right_quadrant) {
				if (top_quadrant) index += top_right << (i++ << 1u);
				if (bottom_quadrant) index += bot_right << (i++ << 1u);
			}
			return index;
		}

		void split()
		{
			if (index_->val + MAX_INDEX > all_trees_->size())
				Logger::error("QuadTree exceeds max allocated trees", Logger::SEVERITY::HIGH);

			auto x = rect_.x;
			auto y = rect_.y;
			auto sub_w = rect_.w / 2.0f;
			auto sub_h = rect_.h / 2.0f;

			// inits 4 quad trees at index and adds entities to the prior tree's children
			trees_[top_left] = all_trees_->at(index_->val++)->init(Rect{ x, y, sub_w, sub_h });
			trees_[top_right] = all_trees_->at(index_->val++)->init(Rect{ x + sub_w, y, sub_w, sub_h });
			trees_[bot_right] = all_trees_->at(index_->val++)->init(Rect{ x + sub_w, y + sub_h, sub_w, sub_h });
			trees_[bot_left] = all_trees_->at(index_->val++)->init(Rect{ x, y + sub_h, sub_w, sub_h });
		}

		void retrieve(Rect col, std::vector<Component::Collider*>& retrieved_objs)
		{
			if (split_)
				for (auto in = index(col);;)
				{
					auto in_tree = trees_[in % MAX_INDEX];
					in_tree->retrieve(col, retrieved_objs);
					in >>= 2;
					if (!in) return;
				}

			for (auto &obj : objects_)
				retrieved_objs.push_back(obj);
		}

		void insert(Component::Collider* obj)
		{
			if (objects_.size() >= max_objects_) {
				split();
				for (auto i = 0u; i < objects_.size(); i++)
				{
					auto col = objects_[i];

					for (auto in = index(col->get_rect());;)
					{
						auto in_tree = trees_[in % MAX_INDEX];
						in_tree->insert(col);
						in >>= 2;
						if (!in) break;
					}
				}
				objects_.clear();
				split_ = true;
			}

			if (split_)
				for (auto in = index(obj->get_rect());;) {
					auto in_tree = trees_[in % MAX_INDEX];
					in_tree->insert(obj);
					in >>= 2;
					if (!in) return;
				}

			objects_.push_back(obj);
		}
	public:
		int num_objects_;
		Component::Int* index_;

		QuadTree()
			: index_(nullptr), all_trees_(nullptr), max_objects_(0), rect_{0}, split_(false), num_objects_(0)
		{}

		QuadTree(Component::QuadTreeVector* all_trees, Component::Int* index, std::size_t max_objects)
			: all_trees_(all_trees), max_objects_(max_objects), index_(index), rect_{0}, split_(false)
		{
			objects_.reserve(max_objects);
		}

		QuadTree* init(Rect rect)
		{
			clear();
			rect_ = rect;
			return this;
		}

		void init(nlohmann::json json, Entity* game) override
		{
			auto root_entity = game->get_nested_child(json["path"]);
			max_objects_ = json["max_objects"];
			int max_trees = json["max_trees"];

			all_trees_ = static_cast<Component::QuadTreeVector*>(root_entity->push_back_component(new Component::QuadTreeVector()));
			all_trees_->reserve(max_trees);
			index_ = static_cast<Component::Int*>(root_entity->push_back_component(new Component::Int()));
			index_->val = 0;
			objects_.reserve(max_objects_);

			for (auto i = 0; i < max_trees; ++i)
				all_trees_->push_back(static_cast<QuadTree*>(root_entity->push_back_component(new Component::QuadTree(all_trees_, index_, max_objects_))));
		}

		void print(std::ostream& os, int level = 0)
		{
			os << std::endl;

			if (trees_[top_left])
			{
				auto t_tl = trees_[top_left]->rect_;
				os << std::string(level, '\t') << "(" << t_tl.x << ", " << t_tl.y << ", "
					<< t_tl.w << ", " << t_tl.h << ")";
				trees_[top_left]->print(os, level + 1);

				auto t_tr = trees_[top_right]->rect_;
				os << std::string(level, '\t') << "(" << t_tr.x << ", " << t_tr.y << ", "
					<< t_tr.w << ", " << t_tr.h << ")";
				trees_[top_right]->print(os, level + 1);

				auto t_br = trees_[bot_right]->rect_;
				os << std::string(level, '\t') << "(" << t_br.x << ", " << t_br.y << ", "
					<< t_br.w << ", " << t_br.h << ")";
				trees_[bot_right]->print(os, level + 1);

				auto t_bl = trees_[bot_left]->rect_;
				os << std::string(level, '\t') << "(" << t_bl.x << ", " << t_bl.y << ", "
					<< t_bl.w << ", " << t_bl.h << ")";
				trees_[bot_left]->print(os, level + 1);

			}

		}

		void add(Component::Collider* col)
		{
			insert(col);
			num_objects_++;
		}

		void add(std::vector<Component::Collider*>& colliders)
		{
			for (Component::Collider* col : colliders)
				add(col);
		}

		void clear()
		{
			num_objects_ = 0;
			objects_.clear();
			split_ = false;
		}

		void all_clear()
		{
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