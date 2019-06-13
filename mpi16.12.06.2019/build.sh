#!/bin/bash

mpic++ master.cpp -o master -lpthread
mpic++ slave.cpp -o slave -lpthread