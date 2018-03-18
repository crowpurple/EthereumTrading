#!/bin/bash

if [[ $# -eq 1 ]]; then
    args=$(find .)
    strings=$1
elif [[ $# -eq 2 ]]; then
    args=$(find $1)
    strings=$2
else
    echo "Usage: ./findstr.sh [path/to/search/from] <string> "
    echo " e.g.: ./findstr.sh /home/clickos/ \"include\" "
    exit 0
fi

for f in $args; do
    if [[ ! -d $f ]]; then
        count=$(grep "$strings" $f | wc -c)
        if [[ $count -gt 0 ]]; then
            echo $f
        fi
    fi
done


