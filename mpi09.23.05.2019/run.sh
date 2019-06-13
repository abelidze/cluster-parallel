#!/bin/bash

srun -n 4 -p main --ntasks-per-node=1 ./task
