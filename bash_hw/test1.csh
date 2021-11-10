#!/usr/bin/env bash
tar -tf practical4_logs.tar.xz | grep -h ‘GET’ logs/logs_2015-01-13-1[0-7]* | awk’{print $2}’ | sort -k1 | uniq -c | \ sort -rk1 | head -n 3