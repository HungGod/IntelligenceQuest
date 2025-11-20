#pragma once
#include "ec/component/system/component-system.h"
#include "logger.h"
#include <stack>

#define MAX_CACHE 64

namespace Component
{
	class SystemCache : public IComponent
	{
		std::array<std::vector<ISystem*>, MAX_CACHE> cache_;
		int pos_;
	public:
		SystemCache()
			: pos_(-1)
		{
			for (auto i = 0; i < MAX_CACHE; ++i)
				cache_[i] = std::vector<ISystem*>();
		}

		void clear_cache()
		{
			pos_ = -1;
		}

		std::vector<ISystem*> get_previous_cache()
		{
			if (pos_ < 0)
				Logger::error("cache is empty", Logger::HIGH);

			return cache_[pos_--];
		}

		void add_to_cache(std::vector<ISystem*> cached_val)
		{
			if (++pos_ >= MAX_CACHE)
				Logger::error("cache is full", Logger::HIGH);
				
			cache_[pos_].clear();
			cache_[pos_].insert(cache_[pos_].end(), cached_val.begin(), cached_val.end());
		}

		std::string get_id() override { return "system_cache"; }
	};
}