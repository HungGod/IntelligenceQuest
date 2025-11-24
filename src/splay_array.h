#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include "logger.h"

template <typename T>
class SplayArray
{
	class Node
	{
	public:
		Node(std::size_t key, T* val)
			: key(key), val(val), left(-1), right(-1)
		{}

		Node(int val)
			: Node(0, val)
		{}

		std::size_t key;
		T* val;
		int left, right;
	};

	int right_rotate(int x)
	{
		int y = node_arr_[x]->left;
		Node* y_node = node_arr_[y].get();
		node_arr_[x]->left = y_node->right;
		y_node->right = x;
		return y;
	}

	int left_rotate(int x)
	{
		int y = node_arr_[x]->right;
		Node* y_node = node_arr_[y].get();
		node_arr_[x]->right = y_node->left;
		y_node->left = x;
		return y;
	}

	int splay(int n, std::size_t key)
	{
		if (n < 0 || node_arr_[n]->key == key)
			return n;

		Node* node = node_arr_[n].get();

		if (node->key > key)
		{
			if (node->left < 0) return n;

			if (node_arr_[node->left]->key > key) // left left rotation
			{
				node_arr_[node->left]->left = splay(node_arr_[node->left]->left, key);

				n = right_rotate(n);
				node = node_arr_[n].get();
			}
			else if (node_arr_[node->left]->key < key)
			{
				node_arr_[node->left]->right = splay(node_arr_[node->left]->right, key);

				if (node_arr_[node->left]->right >= 0)
					node->left = left_rotate(node->left);
			}

			return (node->left < 0) ? n : right_rotate(n);
		}
		else
		{
			if (node->right < 0) return n;

			if (node_arr_[node->right]->key > key)
			{
				node_arr_[node->right]->left = splay(node_arr_[node->right]->left, key);

				if (node_arr_[node->right]->left >= 0)
					node->right = right_rotate(node->right);
			}
			else if (node_arr_[node->right]->key < key)
			{
				node_arr_[node->right]->right = splay(node_arr_[node->right]->right, key);
				n = left_rotate(n);
				node = node_arr_[n].get();
			}

			return (node->right < 0) ? n : left_rotate(n);
		}
	}

	void in_order(int n, int level = 0)
	{
		if (n >= 0)
		{
			in_order(node_arr_[n].left, level + 1);
			std::cout << std::string(level, '\t') << node_arr_[n].key << std::endl;
			in_order(node_arr_[n].right, level + 1);
		}
	}

	int root_;
	std::vector<std::unique_ptr<Node>> node_arr_;
	std::vector<std::unique_ptr<T>> val_arr_;
public:
	SplayArray()
		: root_(-1)
	{}

	~SplayArray() = default;

	T* insert(std::size_t key, T* val)
	{
		if (root_ < 0)
		{
			val_arr_.push_back(std::unique_ptr<T>(val));
			node_arr_.push_back(std::unique_ptr<Node>(new Node(key, val)));
			root_ = (int)node_arr_.size() - 1;
			return val;
		}

		root_ = splay(root_, key);

		if (node_arr_[root_]->key == key)
			Logger::error("SplayArray: inserting the same value", Logger::HIGH);
			
		val_arr_.push_back(std::unique_ptr<T>(val));
		Node* node = new Node(key, val);

		if (node_arr_[root_]->key > key)
		{
			node->right = root_;
			node->left = node_arr_[root_]->left;
			node_arr_[root_]->left = -1;
		}
		else
		{
			node->left = root_;
			node->right = node_arr_[root_]->right;
			node_arr_[root_]->right = -1;
		}

		node_arr_.push_back(std::unique_ptr<Node>(node));
		root_ = (int)node_arr_.size() - 1;
		return val;
	}

	T* push_back(T* val)
	{
		val_arr_.push_back(std::unique_ptr<T>(val));
		return val;
	}

	T* search(std::size_t key)
	{
		root_ = splay(root_, key);

		if (root_ >= 0)
			if (node_arr_[root_]->key == key)
				return node_arr_[root_]->val;

		return nullptr;
	}

	T* get(std::size_t pos)
	{
		if (pos > node_arr_.size())
			return nullptr;

		return node_arr_[pos]->val;
	}

	std::vector<T*> get_list()
	{
		std::vector<T*> list;
		for (auto& v : val_arr_)
			list.push_back(v.get());

		return list;
	}

	template <typename N>
	std::vector<N*> get_casted_list()
	{
		std::vector<N*> list;
		for (auto& v : val_arr_)
			list.push_back(static_cast<N*>(v.get()));

		return list;
	}

	void print()
	{
		std::cout << std::endl;
		in_order(root_);
		std::cout << std::endl;
	}


	std::size_t size()
	{
		return val_arr_.size();
	}

	void deep_print()
	{
		int i = 0;
		if (root_ >= 0)
			std::cout << "root: " << root_ << " key: " << node_arr_[root_]->key << " left: " << node_arr_[root_]->left << " right: " << node_arr_[root_]->right << std::endl;
		for (auto n : node_arr_)
		{
			std::cout << "node " << i++ << ": " << n->key << " left: " << n->left << " right: " << n->right << std::endl;
		}
	}
};