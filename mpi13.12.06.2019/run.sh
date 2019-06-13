#!/bin/bash

srun -n 16 -p main --ntasks-per-node=1 task
