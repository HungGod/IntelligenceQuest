#pragma once
#include <vector>
#include <iostream>
#include <GLFW/glfw3.h>

template <typename T>
class FixedSplayArray
{
	class Node
	{
	public:
		Node()
			: key(0), left(-1), right(-1), touches(0)
		{}

		void init(std::size_t n_key, T& n_val)
		{
			key = n_key;
			val = n_val;
		}

		bool is_leaf()
		{
			return left < 0 && right < 0;
		}

		void change_val(T n_val)
		{
			val = n_val;
		}

		void touch()
		{
			touches++;
		}
		
		void remove()
		{
			touches = 0;
			left = right = -1;
		}

		std::size_t key;
		int touches;
		T val;
		int left, right;
	};

	int right_rotate(int x)
	{
		int y = node_arr_[x].left;
		Node& y_node = node_arr_[y];
		node_arr_[x].left = y_node.right;
		y_node.right = x;
		return y;
	}

	int left_rotate(int x)
	{
		int y = node_arr_[x].right;
		Node& y_node = node_arr_[y];
		node_arr_[x].right = y_node.left;
		y_node.left = x;
		return y;
	}

	int splay(int n, std::size_t key)
	{
		if (n < 0 || node_arr_[n].key == key)
			return n;

		Node& node = node_arr_[n];

		if (node.key > key)
		{
			if (node.left < 0) return n;

			if (node_arr_[node.left].key > key) // left left rotation
			{
				node_arr_[node.left].left = splay(node_arr_[node.left].left, key);

				n = right_rotate(n);
			}
			else if (node_arr_[node.left].key < key)
			{
				node_arr_[node.left].right = splay(node_arr_[node.left].right, key);

				if (node_arr_[node.left].right >= 0)
					node.left = left_rotate(node.left);
			}

			return (node_arr_[n].left < 0) ? n : right_rotate(n);
		}
		else
		{
			if (node.right < 0) return n;

			if (node_arr_[node.right].key > key)
			{
				node_arr_[node.right].left = splay(node_arr_[node.right].left, key);

				if (node_arr_[node.right].left >= 0)
					node.right = right_rotate(node.right);
			}
			else if (node_arr_[node.right].key < key)
			{
				node_arr_[node.right].right = splay(node_arr_[node.right].right, key);
				n = left_rotate(n);
			}

			return (node_arr_[n].right < 0) ? n : left_rotate(n);
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

	void find_least_touched()
	{
		auto youngest = INT16_MAX;
		for (auto i = 0; i < size_; ++i)
		{
			if (node_arr_[i].touches < youngest)
			{
				youngest = node_arr_[i].touches;
				next_ = i;
			}
			node_arr_[i].touches--;
		}
	}

	void remove(std::size_t key)
	{
		if (root_ < 0)
			return;

		root_ = splay(root_, key);

		if (key != node_arr_[root_].key)
		{
			std::cout << "key: " << key << std::endl;
			print();
			throw;
		}
			
		int temp = root_;

		if (node_arr_[root_].left < 0)
			root_ = node_arr_[root_].right;
		else
		{
			root_ = splay(node_arr_[root_].left, key);
			node_arr_[root_].right = node_arr_[temp].right;
		}

		node_arr_[temp].remove();
	}

	void reset_touches()
	{
		for (auto& n : node_arr_)
			n.touches = 0;
	}

	int root_;
	std::size_t size_;
	bool is_full_;
	std::vector<Node> node_arr_;
	int next_;
public:
	FixedSplayArray()
		: root_(-1), size_(0), next_(0), is_full_(false)
	{}

	~FixedSplayArray() = default;

	void resize(std::size_t size)
	{
		size_ = size;
		node_arr_.resize(size_, Node());
	}

	T& insert(std::string key_str, T& val)
	{
		auto key = std::hash<std::string>{}(key_str);
		if (root_ < 0)
		{
			auto& node = node_arr_[next_];
			node.init(key, val);
			node.touch();
			root_ = next_++;
			return val;
		}

		root_ = splay(root_, key);
		node_arr_[root_].touch();

		if (node_arr_[root_].key == key)
		{
			node_arr_[root_].change_val(val);
			return val;
		}

		if (is_full_)
		{
			find_least_touched();
			remove(node_arr_[next_].key);
			root_ = splay(root_, key);
			node_arr_[root_].touch();
		}
			
		auto& node = node_arr_[next_];

		if (node_arr_[root_].key > key)
		{
			node.right = root_;
			node.left = node_arr_[root_].left;
			node_arr_[root_].left = -1;
		}
		else
		{
			node.left = root_;
			node.right = node_arr_[root_].right;
			node_arr_[root_].right = -1;
		}

		node.init(key, val);
		node.touch();
		root_ = next_++;
		
		if (!is_full_ && next_ >= size_)
		{
			reset_touches();
			is_full_ = true;
		}

		return val;
	}

	T& search(std::string key_str)
	{
		auto key = std::hash<std::string>{}(key_str);
		root_ = splay(root_, key);
		node_arr_[root_].touch();

		if (root_ >= 0)
			if (node_arr_[root_].key == key)
				return node_arr_[root_].val;

		return T();
	}

	void print()
	{
		std::cout << std::endl;
		in_order(root_);
		std::cout << std::endl;
	}

	std::size_t get_size()
	{
		return size_;
	}

	void deep_print()
	{
		std::cout << "next: " << next_ << std::endl;
		int i = 0;
		if (root_ >= 0)
			std::cout << "root: " << root_ << " key: " << node_arr_[root_].key << " left: " << node_arr_[root_].left << " right: " << node_arr_[root_].right << " touches: " << node_arr_[root_].touches << std::endl;
		for (auto& n : node_arr_)
		{
			std::cout << "node " << i++ << ": " << n.key << " left: " << n.left << " right: " << n.right << " touches: " << n.touches << std::endl;
		}
	}
};
