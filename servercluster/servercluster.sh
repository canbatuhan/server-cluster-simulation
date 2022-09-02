#!/bin/bash

python compiler.py
echo "---------------------------------------------------------------"
echo "------------------ Server Cluster Simulation ------------------"
echo "---------------------------------------------------------------"
./servercluster
rm servercluster.exe