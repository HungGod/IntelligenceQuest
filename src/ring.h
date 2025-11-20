#pragma once
#include <vector>
#include <memory>

template<typename T>
class Ring
{
	std::vector<std::unique_ptr<T>> ring;
	std::size_t next_, size_;
public:
	Ring()
		: next_(0), size_(0)
	{}

	template <typename N>
	void resize(std::size_t size)
	{
		size_ = size;
		for (auto i = 0; i < size_; ++i)
			ring.push_back(std::unique_ptr<T>(new N()));
	}

	T* get()
	{
		next_ = ++next_ >= size_ ? 0 : next_;
		return ring[next_].get();
	}
};
