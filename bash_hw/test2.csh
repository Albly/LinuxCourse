#!/usr/bin/env bash

tar -tf practical4_logs.tar.xz | find -size +5M -type f -name “*.tsv” | wc -l 
tar -tf practical4_logs.tar.xz | find -size +5M -type f -name “*.tsv” | \ 
xargs tar -cvf /home/alex/fsecourse/target.tar.gz &
