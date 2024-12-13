# Project Description
The disruptive anti-covering location problem (DACLP) aims to find the minimum number of facilities that can be located on a set of potential sites for locating facilities. The facilities on these sites have to be located in such a way that each
pair of facilities are separated by a distance more than R. No two pairs of facilities are supposed to be within the distance of R. DACLP focuses on finding the minimum number of facilities and hence it avoids the “maximal” packing solution of the
anti-covering location problem (ACLP).

In the real world, DACLP plays an important role in solving facility location problems like locating bank branches, nuclear power plants, military defence units, hospitals, and many more. In this project, we aim to develop two hyper-heuristic
approaches namely HH GREEDY and HH RANDOM that can perform on par with the existing heuristic approaches. Our overall approach is being applied to two datasets namely Beasley’s ORLibrary and the TSPLIB. The four low-level heuristic approaches that we have
implemented are Random Addition and Greedy Removal, Greedy Removal and Random Addition, Greedy Removal and Greedy Addition, and Random Addition and Greedy Removal. Each of these low-level heuristic approaches focuses on the
removal and addition of nodes at any given instance. The hyper-heuristic approach will utilise our four low-level heuristics for solving DACLP.

The proposed hyper-heuristic approach has been implemented in C. We have executed our approach on a computer system having an Ubuntu operating system, with a RAM of 4 GB on a 2.00 GHz i3-6006U processor. The instances of nodes up to
100 are executed for 20 seconds. The instances between 100 and 500 are executed for 40 seconds, and the remaining instances are executed for 60 seconds. The proposed approach is much faster in comparison to the DDE and GA approaches.
Our approach has been tested on two datasets, namely Beasley’s OR-Library and the TSPLIB. The highest instance dataset in OR-Library is 1000 and TSPLIB is 1577. Two tables have been formatted for different values of R, which is the
minimum distance. We have reported the least and average values for each R in the table. The values of HH-RAND and HH-GREEDY are similar for most instances except for a few. We can also deduce a comparison between these approaches for
further improvements in the future.

We have 10 multi-starts where an initial solution is generated 10 times, and on each of these initial solutions, we apply low-level heuristics 100 times. HH-RAND at any instance applies one of the low-level heuristics, and HHGREEDY applies all the low-level heuristics available. HH-RAND is much faster
in comparison to HH-GREEDY, for obvious reasons.


