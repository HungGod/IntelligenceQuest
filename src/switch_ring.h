#pragma once
#include <vector>

template<typename T>
class SwitchRing
{
	std::array<std::vector<T>, 2> ring_;
	char next_;
public:
	SwitchRing()
		: next_(0), ring_{ std::vector<T>(), std::vector<T>()}
	{
		ring_[0].reserve(256);
		ring_[1].reserve(256);
	}

	std::vector<T> get()
	{
		ring_[next_].clear();
		next_ = static_cast<char>(!next_);
		return ring_[next_];
	}

	std::vector<T> print_next()
	{
		return ring_[static_cast<char>(!next_)];
	}

	void insert_all_next(std::vector<T> vec)
	{
		auto& next = ring_[static_cast<std::size_t>(!next_)];
		next.insert(next.end(), vec.begin(), vec.end());
	}

	void insert_next(T val)
	{
		ring_[static_cast<std::size_t>(!next_)].push_back(val);
	}
};