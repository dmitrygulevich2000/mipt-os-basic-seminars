#!/bin/bash

if [[ -z $3 ]]; then
    echo "usage: $0 data_file host port"
    exit 1
fi

data_file=$1
host=$2
port=$3

cat $data_file | netcat -N $host $port >/dev/null
# echo "Status: $?"
