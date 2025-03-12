#include "cache.h"

int slow_get_page (int page)
{
    return page;
}

int main()
{
    using PageIdT = int;
    using DataT = int;

    std::vector<PageIdT> pages;
    int pageID;
    int size_prediction;
    int size_cache;

    std::cin >> size_cache;
    if ( !std::cin.good() )
    {
        std::cout << "Error while reading from input sream1" << std::endl;
        return EXIT_FAILURE;
    }

    std::cin >> size_prediction;
    if ( !std::cin.good() ) 
    {
        std::cout << "Error while reading from input sream2" << std::endl;
        return EXIT_FAILURE;
    }

    pages.reserve(size_prediction);

    for (int i = 0; i < size_prediction; ++i)
    {
        PageIdT pageID = 0;

        std::cin >> pageID;
        if ( std::cin.good() ) { pages.push_back(pageID); }
        else
        {
            std::cout << "Error while reading from input sream3" << std::endl;
            return EXIT_FAILURE;
        }
    }

    cacheq::cache <PageIdT, DataT> cur_cache(size_cache);

    std::cout << cur_cache.hits(pages, slow_get_page) << std::endl;
}
