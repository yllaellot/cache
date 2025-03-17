#include <iostream>
#include <list>
#include <ranges>
#include <vector>
#include <unordered_map>
#include <algorithm>

namespace cachep {

template <typename PageIdT, typename DataT>
class perfect_cache
{
    static_assert( std::is_integral_v<PageIdT> || std::is_same_v<PageIdT, std::string>, 
                 "Incorrect Page Id Type. Expected integral or string type");

    using PageT = typename std::pair<PageIdT, int>;
    using List_pages = typename std::list<PageT>;
    using List_pages_it = typename List_pages ::iterator;
    using List_data_it = typename std::list<DataT>::iterator;

public:

    perfect_cache(int cache_size, 
                std::vector<PageIdT> cache_prediction)
    {
        cur_pos = 0;
        size = cache_size;

        prediction.reserve(cache_prediction.size());

        for_each(cache_prediction.begin(), cache_prediction.end(), [&prediction = prediction](PageIdT& pageID)
        {
            prediction.push_back({pageID, -1});
        });

        using VecPageItT = typename std::vector<PageT>::iterator;
        VecPageItT start = prediction.begin();
        VecPageItT end = prediction.end();
        //VecPageItT beg = prediction.begin();
        VecPageItT begs = prediction.begin();

        for_each(prediction.begin(), prediction.end(), [&](PageT& page)
        {
            start = begs;
            begs++;
            start = std::find_if(start + 1, end, [&](auto i){ return i.first == page.first; });
            if ( start != end ) { page.second = std::distance(prediction.begin(), start); }
        });

        farthest_page_ind = 0;
        farthest_page_it = list_pages.begin();
    }

    template <typename FuncT> bool lookup_update(PageT page, const FuncT& slow_get_page)
    {
        if ( hash_pages.find(page.first) != hash_pages.end() )
        {
            if (page.second == -1)
            {
                pages_erase(page);
                data_erase(page);
            }
            else
            {
                if (page.second > farthest_page_ind)
                {
                    farthest_page_ind = page.second;
                    farthest_page_it = hash_pages[page.first];
                }

                hash_pages[page.first]->second =  page.second;
            }
            return true;   
        }

        if ( page.second == -1 )
        {
            return false;
        }

        list_pages.push_front(page);
        hash_pages[page.first] = list_pages.begin();
        List_pages_it erase_aplicant = farthest_page_it;

        if ( list_pages.size() - 1 == size )
        {

            if ( farthest_page_ind < page.second )
            {
                erase_aplicant = list_pages.begin();
            }
            else
            {
                int fpi = 0;
                List_pages_it fpi_it;

                for (auto it = list_pages.begin(); it != list_pages.end(); it++)
                {
                    if ( fpi < it->second && it != farthest_page_it )
                    {
                        fpi = it->second;
                        fpi_it = it;
                    }   
                }

                farthest_page_ind = fpi;
                farthest_page_it = fpi_it;
            }

            pages_erase_IT(erase_aplicant);

            if ( erase_aplicant->first != page.first )
            {
                data_erase_PI(erase_aplicant->first);
                data_push_front(page, slow_get_page);
            }

            return false;
        }

        data_push_front(page, slow_get_page);

        if ( farthest_page_ind < page.second )
        {
            farthest_page_ind = page.second;
            farthest_page_it = list_pages.begin();
        }

        return false;
    }

    template <typename FuncT> int hits(const FuncT& slow_get_page)
    {
        if ( prediction.empty() ) throw "Empty prediction";
        
        int num_hits = 0;

        for (auto page : prediction)
        {
            if ( lookup_update(page, slow_get_page) )
            {
                ++num_hits;
            }
        }

        return num_hits;
    }

private:

    int size;
    int cur_pos;
    int farthest_page_ind;
    List_pages_it farthest_page_it; 
    List_pages list_pages;
    std::list<DataT> list_data;
    std::vector<PageT> prediction;
    std::unordered_map< PageIdT,List_pages_it> hash_pages;
    std::unordered_map< PageIdT, List_data_it> hash_data;

    template <typename FuncT> void data_push_front(PageT page,
                                                   const FuncT& slow_get_page)
    {
        list_data.push_front(slow_get_page(page.first));
        hash_data[page.first] = list_data.begin();
    }

    void pages_erase(PageT erase_aplicant)
    {
        list_pages.erase(hash_pages[erase_aplicant.first]);
        hash_pages.erase(hash_pages.find(erase_aplicant.first));
    }

    void pages_erase_IT(List_pages_it erase_aplicant)
    {
        hash_pages.erase(hash_pages.find(erase_aplicant->first));
        list_pages.erase(erase_aplicant);
    }

    void data_erase(PageT page)
    {
        list_data.erase(hash_data[page.first]);
        hash_data.erase(hash_data.find(page.first));
    }

    void data_erase_PI(PageIdT page)
    {
        list_data.erase(hash_data[page]);
        hash_data.erase(hash_data.find(page));
    }
};

}
