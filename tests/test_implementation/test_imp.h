#pragma once

#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <fstream>
#include "perfect_cache.h"
#include "cache.h"

int slow_get_page_i (int page)
{
    return page;
}

namespace testf
{
    inline int result (const std::string& filename, const std::string& cache_type)
    {
        using PageIdT = int;
        using DataT = int;

        std::ifstream file(filename);
        if ( !file ) { throw "Error opening input stream\n"; }

        int size_cache;
        file >> size_cache;
        if ( !file.good() ) { throw "Error reading from input stream\n"; }

        int size_prediction = 0;
        file >> size_prediction;
        if ( !file.good() ) { throw "Error reading from input stream\n"; }

        std::vector<PageIdT> pages;
        int pageID;

        pages.reserve(size_prediction);

        for (int i = 0; i < size_prediction; ++i)
        {
            PageIdT pageID = 0;

            file >> pageID;
            if ( std::cin.good() ) { pages.push_back(pageID); }
            else { throw "Error reading from input stream\n"; }
        }

        int hits = 0;

        if ( cache_type == "perfect" )
        {
            cachep::perfect_cache <PageIdT, DataT> cur_cache(size_cache, pages.begin(), pages.end());
            hits = cur_cache.hits(pages.begin(), pages.end(), slow_get_page_i);
        }
        else
        {
            cacheq::cache <PageIdT, DataT> cur_cache(size_cache);
            hits = cur_cache.hits(pages, slow_get_page_i);
        }

        return hits;
    }

    int answer ( const std::string& filename )
    {
        std::ifstream answer_file(filename);

        int answer;
        answer_file >> answer;

        return answer;
    }

    void run_test (const std::string& test_name, const std::string& type)
    {
        std::string test_data_dir = "/data/";
        std::string task_data_dir = "tasks/";
        std::string ans_data_dir = "answers/";
        std::string test_path = std::string(TEST_DATA_DIR) + test_data_dir;
        std::string answer_type = ( type == "perfect" ) ? "_perfect.ans" : "_cache.ans";

        int res = result(test_path + task_data_dir + test_name + ".dat", type);
        int ans = answer( test_path + ans_data_dir + test_name + answer_type );

        EXPECT_EQ(res, ans);
    }
}
