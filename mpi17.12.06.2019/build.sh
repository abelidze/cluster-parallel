#!/bin/bash

mpic++ server.cpp -o server -lpthread
mpic++ client.cpp -o client -lpthread