# genetic-inheritance
Simulates a specific trait inheriting down through generations.

This is just another program to keep my 3GL skills exercised.  This application allows a user to create an initial population of N size.  Each person is randomly assigned to be male or female.  The user also chooses an integer to be a genetic trait value of interest within a specificed range of values.  The initial population is randomly seeded with these traits (two values for each person).  For example, a user chooses five as the genetic trait value.  The users also chooses the value range to be between zero and one hundred.  Five will be tracked as the value of interest and be dominant over values zero through four.

After the initial seeding of values for the first generation, new generations are formed based on pairings of males and females.  Each pairing will on average generate 2.3 children one generation down.  Each child will randomly inherit one value from each parent, and be marked as unaffected, carrier or affected.  Pairings continue on down through the desired number of generations.

Summary statistics are reported out on the console, and detailed results are output to results.txt.



-Tim Andrews / Feb 13 2016.
