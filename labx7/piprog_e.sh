#!/usr/bin/bash

export OMP_NUM_THREADS=1

ntrials=1000000
tt=0

until (( $tt ))
do
	ntrials=$((ntrials*10)); echo $ntrials
	
	out=$(./piprog -t $ntrials | grep Elapsed )
	echo $out
        rt=$(echo $out | cut -d " " -f 8)
	rtint=$(echo "scale=0; $rt/1" | bc )
	if [ $rtint -gt 600 ] ; then 
		tt=1
	fi
	//sleep 1
done
      
