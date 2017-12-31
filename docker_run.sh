#!/bin/bash
if [[ $# > 0 ]]; then
    docker run --rm --init --cap-add=SYS_PTRACE -p "127.0.0.1:12345:12345" --name "$1" -it "tharina/$1"
else
    echo No module specified.
fi
