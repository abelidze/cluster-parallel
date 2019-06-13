#!/bin/bash

srun -n 8 -p ddr --ntasks-per-node=8 task
