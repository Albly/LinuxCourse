#!/usr/bin/bash
for x in 1 2 4
do
    cat runpi_b.sh | sed "/cpus/s/=.*$/=$x/" | sbatch
done
~                                                                               
~         
