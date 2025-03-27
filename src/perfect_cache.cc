#include "perfect_cache.h"
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
        std::cout << "Error reading from input stream" << std::endl;
        return EXIT_FAILURE;
    }

    std::cin >> size_prediction;
    if ( !std::cin.good() ) 
    {
        std::cout << "Error reading from input stream" << std::endl;
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
            std::cout << "Error reading from input stream" << std::endl;
            return EXIT_FAILURE;
        }
    }

    cachep::perfect_cache <PageIdT, DataT> cur_cache(size_cache, pages.begin(), pages.end());
    int hits = cur_cache.hits(pages.begin(), pages.end(), slow_get_page);

    std::cout << hits << std::endl;
}