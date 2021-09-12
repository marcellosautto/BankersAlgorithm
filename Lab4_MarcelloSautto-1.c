#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int* resourceVector, * availableVector,
** maxClaimsArray, ** allocationArray, ** needArray,
numProcesses, numResources;

bool* safelySequenced;

void initializeGraph();
void determineSafeSequence();
void freeMemory();

int main() 
{

	int input = 0;
	bool containsData = false;

	do {
		if (input == 3)
			break;

		else if (input == 2)
			if (containsData)
				determineSafeSequence();
			else
				printf("\nNo Data Entered");

		else if (input == 1) {
			if (containsData)
				freeMemory();

			initializeGraph();
			containsData = true;
		}

	} while (printf("\n\nBanker's Algorithm\n-------------------\n1) Enter Parameters\n2) Determine Safe Squence\n3) Quit Prgoram\n\nEnter Selection: ") && scanf("%d", &input));

	printf("\nQuitting Program...\n\n");
	freeMemory();
	return 0;
};

void initializeGraph()
{

	printf("Enter Number Of Processes: ");
	scanf("%d", &numProcesses);
	printf("Enter Number Of Resources: ");
	scanf("%d", &numResources);

	resourceVector = (int*)malloc(numResources * sizeof(int)); //indicates total amount of each resource
	availableVector = (int*)malloc(numResources * sizeof(int)); //indicates avaliable resources
	safelySequenced = (bool*)malloc(numProcesses * sizeof(bool)); //flags when certain processes have safely sequenced
	maxClaimsArray = (int**)malloc(numProcesses * sizeof(int*)); //defines maximum demand of each process
	allocationArray = (int**)malloc(numProcesses * sizeof(int*)); //indicates how many instances of resources a process is currently allocated with
	needArray = (int**)malloc(numProcesses * sizeof(int*)); //indicates if a certain process needs more resources to complete its task

	/*****INITIALIZE AND SIZE VECTORS*****/

	for (int i = 0; i < numResources; i++)
	{
		resourceVector[i] = 0;
		availableVector[i] = 0;
	}
	for (int i = 0; i < numProcesses; i++)
	{
		maxClaimsArray[i] = (int*)malloc((numResources) * sizeof(int));
		allocationArray[i] = (int*)malloc((numResources) * sizeof(int));
		needArray[i] = (int*)malloc((numResources) * sizeof(int));
		safelySequenced[i] = false;

		for (int j = 0; j < numResources; j++)
		{
			maxClaimsArray[i][j] = 0;
			allocationArray[i][j] = 0;
			needArray[i][j] = 0;
		}
	}

	/*****DEFINE PARAMETERS FOR RESOURCE VECTOR, ALLOCATION ARRAY, AND MAX CLAIMS ARRAY*****/

	printf("Enter number of units for resources (r0 to r%d%s", numResources - 1, "): ");
	for (int i = 0; i < numResources; i++)
		scanf("%d", &resourceVector[i]);

	for (int i = 0; i < numProcesses; i++)
	{
		printf("Enter maximum number of units process p%d%s%d%s", i, " will request from each resource (r0 to r", numResources - 1, "): ");
		for (int j = 0; j < numResources; j++)
			scanf("%d", &maxClaimsArray[i][j]);
	}

	for (int i = 0; i < numProcesses; i++)
	{
		printf("Enter number of units of each resource (r0 to r%d%s%d%s", numResources - 1, ") allocated to process p", i, "): ");
		for (int j = 0; j < numResources; j++)
			scanf("%d", &allocationArray[i][j]);
	}

	/****SET AVAILABLE VECTOR AND NEED ARRAY*****/

	//calculates number of available resources by subtracting the resource units 
	//by the number of units allocated to the process
	for (int i = 0; i < numResources; i++)
	{

		for (int j = 0; j < numProcesses; j++)
			availableVector[i] += allocationArray[j][i];

		availableVector[i] = resourceVector[i] - availableVector[i];
	}

	//calculates how many resources processes may need when they perform their execution 
	//by subtracting the maximum demand of each process by the resources allocated to that 
	//specific process
	for (int i = 0; i < numProcesses; i++)
		for (int j = 0; j < numResources; j++)
			needArray[i][j] = maxClaimsArray[i][j] - allocationArray[i][j];

	/*****PRINT CONTAINERS*****/

	printf("\nResources:\n");
	for (int i = 0; i < numResources; i++)
	{
		printf("\tr%d", i);
	}
	printf("\n");
	for (int i = 0; i < numResources; i++)
	{
		printf("\t%d", resourceVector[i]);
	}

	printf("\n\nAvailable:\n");
	for (int i = 0; i < numResources; i++)
	{
		printf("\tr%d", i);
	}
	printf("\n");
	for (int i = 0; i < numResources; i++)
	{
		printf("\t%d", availableVector[i]);
	}

	printf("\n\nMax Claim:\n");
	for (int i = 0; i < numResources; i++)
	{
		printf("\tr%d", i);
	}
	for (int i = 0; i < numProcesses; i++)
	{
		printf("\np%d", i);

		for (int j = 0; j < numResources; j++)
			printf("\t%d", maxClaimsArray[i][j]);
	}

	printf("\n\nAllocated:\n");
	for (int i = 0; i < numResources; i++)
	{
		printf("\tr%d", i);
	}
	for (int i = 0; i < numProcesses; i++)
	{
		printf("\np%d", i);

		for (int j = 0; j < numResources; j++)
			printf("\t%d", allocationArray[i][j]);
	}

	printf("\n\nNeed:\n");
	for (int i = 0; i < numResources; i++) 
	{
		printf("\tr%d", i);
	}
	for (int i = 0; i < numProcesses; i++)
	{
		printf("\np%d", i);

		for (int j = 0; j < numResources; j++)
			printf("\t%d", needArray[i][j]);
	}
};

//determines if processes can safely sequence
void determineSafeSequence() {

	for (int fails = 0; fails < numProcesses; fails++)
	{
		for (int i = 0; i < numProcesses; i++)
		{
			if (safelySequenced[i] == false)
			{

				printf("\nChecking: < ");
				for (int j = 0; j < numResources; j++)
					printf("%d%s", needArray[i][j], " ");
				printf("> <= < ");
				for (int j = 0; j < numResources; j++)
					printf("%d%s", availableVector[j], " ");
				printf("> ");

				for (int j = 0; j < numResources; j++)
				{
					if (needArray[i][j] > availableVector[j])
					{
						safelySequenced[i] = false;
						break;
					}
					else
						safelySequenced[i] = true;
				}

				//allow allocated resources of a process to be availble once the process safely sequences
				if (safelySequenced[i] == true)
				{
					printf(":p%d%s", i, " safely sequenced");

					for (int j = 0; j < numResources; j++)
						availableVector[j] = availableVector[j] + allocationArray[i][j];
				}
			}
		}
	}

	for (int i = 0; i < numProcesses; i++) //sequence is determined safe if no processes failed to sequence safely
	{
		if (safelySequenced[i] == false) 
		{
			printf("\n\nNo safe sequence found");
			break;
		}
	}

};

void freeMemory() 
{

	for (int i = 0; i < numProcesses; i++) 
	{
		free(maxClaimsArray[i]);
		free(allocationArray[i]);
		free(needArray[i]);
	}

	free(maxClaimsArray);
	free(allocationArray);
	free(needArray);
	free(resourceVector);
	free(availableVector);
	free(safelySequenced);
};

