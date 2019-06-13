#!/bin/bash

echo 'VECTOR_SIZE = 1000000'
for ((i = 1; i < 9; i++))
do
	srun -n $i -p main ./task 1000000 &
	wait
done

echo
echo

echo 'VECTOR_SIZE = 50000000'
for ((i = 1; i < 9; i++))
do
	srun -n $i -p main ./task 50000000 &
	wait
done
