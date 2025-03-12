#include <iostream>
#include <list>
#include <type_traits>
#include <unordered_map>
#include <vector>

#define COEFF_SIZE_IN_MAIN 5

namespace cacheq {

template <typename DataT, typename PageIdT>
class cache
{
    static_assert( std::is_integral_v<PageIdT> || std::is_same_v<PageIdT, std::string>, 
                 "Incorrect Page Id Type. Expected integral or string type");

    using PageHashT = std::unordered_map<PageIdT, typename std::list<PageIdT>::iterator>;

public:

    explicit cache(int size)
    {
        if ( size < 4 )
        {
            size_in = std::max(size / 2, 1);
            size_out = size / 2;
            size_main = size - size_in - size_out;
        }
        else if ( size < 11 )
        {
            size_in = size / 3;
            size_out = size / 3;
            size_main = size - size_in - size_out;
        }
        else
        {
            size_in = size / COEFF_SIZE_IN_MAIN;
            size_main = size / COEFF_SIZE_IN_MAIN;
            size_out = size - size_in - size_main;
        }
    }

    template <typename FuncT> bool lookup_update (PageIdT page, const FuncT& slow_get_page)
    {
        if ( hash_main.find(page) != hash_main.end() || hash_in.find(page) != hash_in.end() )
        {
            return true;
        }

        if ( hash_out.find(page) != hash_out.end() )
        {
            if ( size_main != 0 )
            {
                main_push_front(page);
                out.erase(hash_out[page]);
                hash_out.erase(hash_out.find(page));
            }

            return true;
        }

        in_push_front(page, slow_get_page);

        return false;
    }

    template <typename FuncT> int hits(std::vector<PageIdT>& prediction,
                                       const FuncT& slow_get_page)
    {
        if ( (prediction).empty() ) throw "Empty prediction";

        int num_hits = 0;

        for (PageIdT page : prediction)
        {
            if ( lookup_update(page, slow_get_page) )
            {
                ++num_hits;
            }
        }

        return num_hits;
    }

private:
    std::list<PageIdT> in;
    std::list<PageIdT> out;
    std::list<PageIdT> main;
    std::list<DataT> Data;

    PageHashT hash_in;
    PageHashT hash_out;
    PageHashT hash_main;

    std::unordered_map<PageIdT, typename std::list<DataT>::iterator> hash_data;

    int size_in;
    int size_out;
    int size_main;

    void main_push_front(PageIdT page)
    {
        if ( size_main == 0 )
        {
            return;
        }

        if ( main.size() == size_main )
        {
            hash_main.erase(hash_main.find(main.back()));
            data_erase(main.back());
            main.pop_back();
        }

        main.push_front(page);
        hash_main[page] = main.begin();
    }

    void out_push_front(PageIdT page)
    {
        if ( size_out == 0 )
        {
            return;
        }

        if ( out.size() == size_out )
        {
            hash_out.erase(hash_out.find(out.back()));
            data_erase(out.back());
            out.pop_back();
        }

        out.push_front(page);
        hash_out[page] = out.begin();
    }

    template <typename FuncT> void in_push_front(PageIdT page, const FuncT& slow_get_page)
    {
        if ( in.size() == size_in )
        {
            out_push_front(in.back());
            hash_in.erase(hash_in.find(in.back()));
            in.pop_back();
        }

        in.push_front(page);
        hash_in[page] = in.begin();

        data_push_front(page, slow_get_page);
    }

    template <typename FuncT> void data_push_front(PageIdT page, const FuncT& slow_get_page)
    {
        Data.push_front(slow_get_page(page));
        hash_data[page] = Data.begin();
    }


    void data_erase(int page)
    {
        Data.erase(hash_data[page]);
        hash_data.erase(hash_data.find(page));
    }
};

}
