#!/usr/bin/bash
for x in 1 2 4
do
	export OMP_NUM_THREADS=$x
	./piprog3
done
