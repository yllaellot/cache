#!/bin/bash

for file in ./data/tasks/*.dat ; do
    echo ________________________;
    s=${file##*/}; 
    echo Test name:  ${s%.dat};
    echo Cache hits: ; 
    res=$(./../build/cache < $file)
    echo $res;
    echo Perfect cache hits: ;
    resp=$(./../build/perfect_cache < $file)
    echo $resp;
    echo ________________________;
    answ=$(cat ./data/answers/${s%.dat}_cache.ans);
    answp=$(cat ./data/answers/${s%.dat}_perfect.ans);
    if [ "$answ" == "$res" ] && [ "$answp" == "$resp" ]; then
    	echo "passed"
    fi
done
