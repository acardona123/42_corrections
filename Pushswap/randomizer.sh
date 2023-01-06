#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 MIN MAX"
    exit 1
fi

min=$1
max=$2

if [ $min -gt $max ]; then
    echo "Error: MIN must be less than or equal to MAX"
    exit 1
fi

for i in $(seq $min $max | shuf); do
    echo -n $i ''
done
