#!/bin/bash
if [[ $# > 0 ]]; then
    make "$@"
    if [[ $? = 0 ]]; then
        docker build -t tharina/$1 --build-arg flag=./flags/$1.flag --build-arg module=$1 .
    else
        echo Build failed
    fi
else
    echo No module specified.
fi
