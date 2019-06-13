#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: ./run_client.sh <PORT_NAME>"
else
    mpiexec -n 1 client $1
fi
