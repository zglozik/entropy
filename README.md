
Entropy
=======

Building
--------

You need the following to build the application:

- Linux (tested on Ubuntu)
- gcc
- make

To build the program execute the following from the terminal:

	$ cd .../entropy
	$ make
	
This will create an executable called *entropy* in the current directory, which implements
the algorithm documented [here](https://en.wikipedia.org/wiki/Limiting_density_of_discrete_points)


Running
-------

The input file represents an n by m table with each column representing a probability 
distribution function sampled at a given frequency. Entries are separated by commas and
lines are terminated by #, e.g.:

	0.2,0.4,0.1#
	0.1.,0.1,0.2#
	...
	0.6.,0.2,0.42#

To execute the application you need to provide the frequency and the name of the input file:

	$ ./entropy 10 trial1.fre
	1. entropy: 3.75356
	2. entropy: 4.33247
	3. entropy: 4.1462
	4. entropy: 4.02534
	5. entropy: 4.10443
	...

-------------
Zoltan Glozik
