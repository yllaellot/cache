#pragma once

#include <deque>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>

namespace cachep {

template <typename PageIdT, typename DataT>
class perfect_cache
{
    static_assert( std::is_integral_v<PageIdT> || std::is_same_v<PageIdT, std::string>, 
                 "Incorrect Page Id Type. Expected integral or string type");

    using Index = size_t;
    using PageT = typename std::pair<PageIdT, DataT>;
    using ListIndex = typename std::deque<Index>;
    using VecIterator = typename std::vector<PageIdT>::iterator;
    using CacheIterator = typename std::map<Index, PageT>::iterator;
    using PredictionIterator = typename std::unordered_map<PageIdT, ListIndex>::iterator;


public:

    perfect_cache (const int& cache_size, const VecIterator& begin, const VecIterator& end) :
    size_(0),
    hits_(0),
    capacity_(cache_size)
    {
        Index index = 0;

        std::for_each(begin, end, [&](PageIdT& pageID)
        {
            if (!prediction_.count(pageID))
            {
                prediction_.emplace(pageID, ListIndex{index});
            }
            else prediction_[pageID].push_back(index);

            index++;
        });
    }

    template <typename FuncT> bool lookup_update(PageIdT pageID, const FuncT& slow_get_page)
    {
        if (cache_.count(prediction_[pageID].front()))
        {
            cache_update(pageID);

            hits_++;
            return true;
        }

        DataT page_data = slow_get_page(pageID);

        add_page(pageID, page_data);

        return false;
    }

    template <typename FuncT> size_t hits(const VecIterator& begin,
                                          const VecIterator& end,
                                          const FuncT& slow_get_page)
    {
        if ( prediction_.empty() ) throw "Empty prediction";
        
        std::for_each(begin, end, [&](PageIdT& pageID){ lookup_update(pageID, slow_get_page); });

        return hits_;
    }

private:

    int size_;
    int hits_;
    int capacity_;

    std::unordered_map<PageIdT, ListIndex> prediction_;
    std::map<Index, PageT> cache_;


    void cache_update (PageIdT& pageID)
    {
        CacheIterator curr_cache_pos_it = cache_.find(prediction_[pageID].front());
        PredictionIterator curr_prediction_pos_it = prediction_.find(pageID);
        ListIndex& indexes_list = curr_prediction_pos_it->second;

        if (!indexes_list.empty()) indexes_list.pop_front();

        if (!indexes_list.empty())
        {
            Index next_index = indexes_list.front();

            cache_.emplace(next_index, curr_cache_pos_it->second);
            cache_.erase(curr_cache_pos_it);
        }
        else
        {
            cache_.erase(curr_cache_pos_it);
            prediction_.erase(curr_prediction_pos_it);
            size_--;
        }
    }

    bool is_full ()
    {
        return size_ == capacity_ ? true : false;
    }

    void erase_page (ListIndex& prediction_new)
    {
        CacheIterator farthest_page = std::prev(cache_.end());
        Index farthest_index = farthest_page->first;
        Index farthest_index_new = prediction_new.front();

        if (farthest_index_new > farthest_index)
        {
            prediction_new.pop_front();

            return;
        }

        cache_.erase(farthest_page);

        size_--;
    }

    void add_page (PageIdT pageID, DataT data)
    {
        ListIndex& prediction_new = prediction_.at(pageID);

        if (!prediction_new.empty())  prediction_new.pop_front();
    
        if (prediction_new.empty()) return;

        if (is_full()) 
        {
            CacheIterator farthest_page = std::prev(cache_.end());
            Index farthest_index = farthest_page->first;
            Index farthest_index_new = prediction_new.front();

            if (farthest_index_new > farthest_index)
            {
                prediction_new.pop_front();

                return;
            }

            cache_.erase(farthest_page);

            size_--;
        }

        cache_.emplace(prediction_new.front(), std::make_pair(pageID, data));

        size_++;
    }
};
}
