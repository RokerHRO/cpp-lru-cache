/* 
 * File:   lrucache.hpp
 * Author: Alexander Ponomarev, Lars H. Rohwedder
 *
 * Created on June 20, 2013, 5:09 PM, edited 2024-03-02 by LR
 */

#ifndef _LRUCACHE_HPP_INCLUDED_
#define	_LRUCACHE_HPP_INCLUDED_

#include <unordered_map>
#include <list>
#include <cstddef>
#include <stdexcept>

namespace cache {

template<typename key_t, typename value_t>
class lru_cache
{
public:
	typedef typename std::pair<key_t, value_t> key_value_pair_t;
	typedef typename std::list<key_value_pair_t>::iterator list_iterator_t;

	explicit lru_cache(size_t max_size = 10)
	: _max_size(max_size)
	{ }
	
	void put(const key_t& key, const value_t& value)
	{
		auto it = map.find(key);
		list.push_front(key_value_pair_t(key, value));
		if (it != map.end())
		{
			list.erase(it->second);
		}
		map[key] = list.begin();
		
		if (map.size() > _max_size)
		{
			auto last = list.rbegin();
			map.erase(last->first);
			list.pop_back();
		}
	}
	
	const value_t& get(const key_t& key)
	{
		auto it = map.find(key);
		if (it == map.end())
		{
			throw std::range_error("There is no such key in cache");
		} else {
			list.splice(list.begin(), list, it->second);
			return it->second->second;
		}
	}
	
	bool exists(const key_t& key) const noexcept
	{
		return map.find(key) != map.end();
	}
	
	size_t size() const noexcept { return map.size(); }
	
private:
	std::list<key_value_pair_t> list;
	std::unordered_map<key_t, list_iterator_t> map;
	size_t _max_size;
};

} // namespace cache

#endif	/* _LRUCACHE_HPP_INCLUDED_ */

