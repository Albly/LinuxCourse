#!/usr/bin/env bash

tar -xf practical4_data_for_science.tar.xz 
ls -lsh | sort -k1 | tail -n 4
grep -iwh 'bad' [0-99].txt | awk '{print $2}' | tr ',' '\n' | tr -d ' ' | sort -k1 | uniq -iu | while read line ; \ do grep -iwo $line [6,5,2].txt ; done | tr ':' ' ' | sort -k1