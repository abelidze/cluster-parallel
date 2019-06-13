#!/bin/bash

mpic++ task.cpp -o task $(python-config --cflags --ldflags)
# mpic++ task.cpp -o task -I$HOME/pkg/matplotlib/include/python2.7 -L$HOME/pkg/matplotlib/lib/python2.7/config -lpython2.7 -lpthread -export-dynamic
# g++ task.cpp -o task $(python-config --cflags --ldflags)