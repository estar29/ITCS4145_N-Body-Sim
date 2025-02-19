#!/bin/bash

# Evan Stark - February 11th 2025 - ITCS 4145 001
# Slurm script to benchmark n-body-sim.cpp on Centaurus.

#SBATCH --partition=Centaurus
#SBATCH --mem=16GB
#SBATCH --time=02:00:00

g++ n-body-sim.cpp -o n-body-output
./output 8 200 5000000 10000
./output 100 1 10000 100
./output 1000 1 10000 100
