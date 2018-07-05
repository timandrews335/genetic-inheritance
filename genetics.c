/* hello there
Simulates a specific trait inheriting down through generations.

This is just another program to keep my 3GL skills exercised. This application allows a user to create an initial population of N size. Each person is randomly assigned to be male or female. The user also chooses an integer to be a genetic trait value of interest within a specificed range of values. The initial population is randomly seeded with these traits (two values for each person). For example, a user chooses five as the genetic trait value. The users also chooses the value range to be between zero and one hundred. Five will be tracked as the value of interest and be dominant over values zero through four.

After the initial seeding of values for the first generation, new generations are formed based on pairings of males and females. Each pairing will on average generate 2.3 children one generation down. Each child will randomly inherit one value from each parent, and be marked as unaffected, carrier or affected. Pairings continue on down through the desired number of generations.

Summary statistics are reported out on the console, and detailed results are output to results.txt.

-Tim Andrews / Feb 13 2016.
tfnpc
*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//Definition of a person
typedef struct {
    int personID;
    int geneValue1;
    int geneValue2;
    int sex; // 0 = M, 1 = F   
    int generation;
    int mate;
    int isAffected; //0 = not affected, 1 = carrier, 2 = affected
    int parent1;
    int parent2;
} person;


//Definition of a collection of persons
typedef struct {
    person *people;
    int count;
    int capacity;
} PersonCollection;

//Initialize a person collection
void CreatePersonCollection(PersonCollection *p, int capacity)
{
    p->count = 0;
    p->capacity = capacity;
    p->people = malloc(sizeof(person) * p->capacity);
}

//Add a person to the collection
void AddPerson(PersonCollection *p, person per) 
{
  p->people[p->count++] = per;
}

//Set the mate of the person to a different person
void SetMate(PersonCollection *p, int person1, int person2) 
{
  p->people[person1].mate = p->people[person2].personID;
}


//Return a person from the collection
person GetPerson(PersonCollection *p, int index) 
{
  return p->people[index];
}


//Free a person collection
void ReleasePeople(PersonCollection *p) 
{
  free(p->people);
}


//Simulate creating random people, gene values, and their children, etc.
int simulate(int numberOfGenerations, int numberOfPeople, int reset, PersonCollection *yourPeople, int numberOfPossibleValues, int geneValueOfInterest)
{
    static int howManySoFar = 1;
    static int personID;

    if (reset == 1)
    {
        howManySoFar = 1;
    }

    //Create some random initial people for the array, if we currently have no people
    if (yourPeople->count == 0)
    {
        int counter;
        for(counter = 0; counter < numberOfPeople; counter++)
        {
            ++personID;
            person p;
            p.personID = personID;
            p.sex = rand() % 2;
            p.geneValue1 = rand() % (numberOfPossibleValues + 1);
            p.geneValue2 = rand() % (numberOfPossibleValues + 1);
            p.generation = 1;
            p.mate = -1;
            p.parent1 = -1;
            p.parent2 = -1;
            p.isAffected = 0;
            if (p.geneValue1 == geneValueOfInterest &&  p.geneValue2 == geneValueOfInterest) {p.isAffected = 2;} //Affected
            if (p.geneValue1 == geneValueOfInterest &&  p.geneValue2 != geneValueOfInterest && p.geneValue1 > p.geneValue2) {p.isAffected = 2;} //Affected
            if (p.geneValue2 == geneValueOfInterest &&  p.geneValue1 != geneValueOfInterest && p.geneValue2 > p.geneValue1) {p.isAffected = 2;} //Affected
            if (p.geneValue1 == geneValueOfInterest &&  p.geneValue2 != geneValueOfInterest && p.geneValue1 < p.geneValue2) {p.isAffected = 1;} //Carrier
            if (p.geneValue2 == geneValueOfInterest &&  p.geneValue1 != geneValueOfInterest && p.geneValue2 < p.geneValue1) {p.isAffected = 1;} //Carrier
            AddPerson(yourPeople, p);
        }
    }

    //Pair up our people
    int i;
    int j;
    int finalCount = yourPeople->count; //Final count so far that we want to loop until.  We will be adding more people to the collection
    
    for(i = 0; i < finalCount; i++)
    {
        //If the person does not already have a mate    
        if (GetPerson(yourPeople, i).mate == -1)
        {        
            for(j = i; j < finalCount; j++)
            {
                //Male and female can mate
                if(GetPerson(yourPeople, i).sex != GetPerson(yourPeople, j).sex )
                {         
                    //If the person does not already have a mate
                    if(GetPerson(yourPeople, j).mate == -1)
                    {
                        //If the person is not one in the same, and our people are within the same generation
                        if(j != i && GetPerson(yourPeople, i).generation == GetPerson(yourPeople, j).generation)
                        {
                            //Store the mate info for both people                        
                            SetMate(yourPeople, i, j);
                            SetMate(yourPeople, j, i);

                            //Create some children, they will have either 2 or 3 children (2.3 on average)
                            int howManyChildren = rand() % 10;
                            if(howManyChildren < 7)
                            {
                                howManyChildren = 2;
                            }
                            else {howManyChildren = 3;}

                            int counter;
                            for(counter = 0; counter < howManyChildren; counter++)
                            {
                                ++personID;
                                person p;
                                p.personID = personID;
                                p.sex = rand() % 2;
                                p.generation = GetPerson(yourPeople, j).generation + 1;
                                p.mate = -1;
                                int fatherNumber = rand() % 2;
                                int motherNumber = rand() % 2;
                                if (fatherNumber == 0){p.geneValue1 = GetPerson(yourPeople, i).geneValue1;} else {p.geneValue1 = GetPerson(yourPeople, i).geneValue2;}
                                if (motherNumber == 0){p.geneValue2 = GetPerson(yourPeople, j).geneValue1;} else {p.geneValue2 = GetPerson(yourPeople, j).geneValue2;}
                                p.parent1 = GetPerson(yourPeople, i).personID;
                                p.parent2 = GetPerson(yourPeople, j).personID;
                                p.isAffected = 0;
                                if (p.geneValue1 == geneValueOfInterest &&  p.geneValue2 == geneValueOfInterest) {p.isAffected = 2;} //Affected
                                if (p.geneValue1 == geneValueOfInterest &&  p.geneValue2 != geneValueOfInterest && p.geneValue1 > p.geneValue2) {p.isAffected = 2;} //Affected
                                if (p.geneValue2 == geneValueOfInterest &&  p.geneValue1 != geneValueOfInterest && p.geneValue2 > p.geneValue1) {p.isAffected = 2;} //Affected
                                if (p.geneValue1 == geneValueOfInterest &&  p.geneValue2 != geneValueOfInterest && p.geneValue1 < p.geneValue2) {p.isAffected = 1;} //Carrier
                                if (p.geneValue2 == geneValueOfInterest &&  p.geneValue1 != geneValueOfInterest && p.geneValue2 < p.geneValue1) {p.isAffected = 1;} //Carrier
                                AddPerson(yourPeople, p);
                            }

                            //Break once a mate is found - no need to travel across the world to find a mate
                            break;
                        }
                    }
                }
            }
        }
    }
    
       

    ++howManySoFar;
    //printf("how many so far?:  %d\n", howManySoFar);


    //Go next
    if(howManySoFar < numberOfGenerations)
    {
        simulate(numberOfGenerations,numberOfPeople,0,yourPeople,numberOfPossibleValues,geneValueOfInterest);
    }
    return 0;
}

int main()
{
    
    int populationCount = 500;
    int numberOfGenerations = 5;
    int numberOfPossibleGeneValues = 20;
    int geneValueOfInterest = 10;
    int counter;

    //Prompt the user for input
    printf("\n");
    printf("What should the initial population be?  ");
    scanf ("%d", &populationCount);
    printf("\n");
    printf("How many generations should we simulate?  ");
    scanf ("%d", &numberOfGenerations);
    printf("\n");
    printf("The trait we are interested in looking at should be a number between 1 and N.  The smaller N, the more initial people will start out with the trait.  What should N be?  ");
    scanf ("%d", &numberOfPossibleGeneValues);
    printf("\n");
    printf("Now, what number, between 1 and %d should the trait be represented by?  This trait will dominant over numbers smaller than it?  ", numberOfPossibleGeneValues);
    scanf ("%d", &geneValueOfInterest);
    printf("\n");


    //Initialize random numbers
    srand(time(NULL));

    //Initialize the collection of people.  We will create enough slots for number of generations, starting at populationCount with 2.3 children
    PersonCollection myPeople;

        //Figure out how many iterations we need
        int i = 1;
        double currentGeneration = populationCount; 
        double totalPop = populationCount;
        double previous = populationCount;
        for (i = 2; i <= (numberOfGenerations + 1); i++)
        {
            currentGeneration = ((previous/2) * 2.3);
            totalPop = totalPop + currentGeneration;
            previous = currentGeneration;    
        }

    CreatePersonCollection(&myPeople, totalPop);

    //Print start time
    time_t starttime;
    time(&starttime);
    printf("%s", ctime(&starttime));
    //Start simulation
    simulate(numberOfGenerations, populationCount, 1, &myPeople,numberOfPossibleGeneValues,geneValueOfInterest);


    //Print out our parameters
    printf("We start with a population of %d people.  For our gene of interest, we have have %d possible values.  We are interested in value %d.  Values higher are dominant, and values lower are recessive.\n\n", populationCount, numberOfPossibleGeneValues, geneValueOfInterest);

    //Print out summary results
    int previousGeneration = 1;
    int generationCount = 0;
    int carriers = 0;
    int affected = 0;
    for(i = 0; i < myPeople.count; i++)
    {
        //We are on a new generation
        if(previousGeneration != GetPerson(&myPeople, i).generation)
        {
            printf("Generation %d had %d total people.  %d were carriers and %d were affected\n", previousGeneration, generationCount, carriers, affected);
            generationCount = 0;
            carriers = 0;
            affected = 0;
        }        
        generationCount += 1;
        if(GetPerson(&myPeople, i).isAffected == 2){affected += 1;}
        if(GetPerson(&myPeople, i).isAffected == 1){carriers += 1;} 
        previousGeneration = GetPerson(&myPeople, i).generation;
    }
    printf("Generation %d had %d total people.  %d were carriers and %d were affected\n", previousGeneration, generationCount, carriers, affected);
    generationCount = 0;
    carriers = 0;
    affected = 0;       
            

    //Print out our people to a text file
    FILE * results;
    results = fopen("results.txt", "w");
    int k;
    fprintf(results, "PersonID,GeneValue1,GeneValue2,Sex,Generation,Mate,isAffected,Parent1,Parent2\n");
    for(k = 0; k < myPeople.count; k++)
    {
        fprintf(results, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n", GetPerson(&myPeople, k).personID,GetPerson(&myPeople, k).geneValue1,GetPerson(&myPeople, k).geneValue2, GetPerson(&myPeople, k).sex, GetPerson(&myPeople, k).generation, GetPerson(&myPeople, k).mate,GetPerson(&myPeople, k).isAffected,GetPerson(&myPeople, k).parent1,GetPerson(&myPeople, k).parent2);
    }
    fclose(results);

    //Dispose of the collection of persons
    ReleasePeople(&myPeople);

    //Print end time
    time_t endtime;
    time(&endtime);
    printf("%s", ctime(&endtime));

    return 0;
}

//cl genetics.c
//genetics > results.txt

