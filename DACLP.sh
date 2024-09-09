#!/bin/bash

# if segmentation fault on line 10 then use this command "sudo sync; echo 3 | sudo tee /proc/sys/vm/drop_caches"
# Set stack size limit
ulimit -s unlimited

# Compile the program with proper output name. 
gcc -o a.out Main.c -lm 

# Execute the program
./a.out

# Make the script executable by running chmod +x DACLP.sh for permission and execute it by ./DACLP.sh

