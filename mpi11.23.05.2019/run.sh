#!/bin/bash

srun -n 8 -p main --ntasks-per-node=1 task
