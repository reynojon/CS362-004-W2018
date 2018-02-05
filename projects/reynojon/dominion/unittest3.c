/*
 * unittest3.c
 *
 * Tests the functionality of the whoseTurn function by checking multiple values
 * 
 * Costs verified at http://wiki.dominionstrategy.com/index.php/List_of_cards
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <stdio.h>

void assertInts(int a, int b, int *count);

int main() {
    int seed = 1000;
    int numPlayers = 2;
	struct gameState G;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, treasure_map};
	int testFails = 0; //Will remain zero if all tests pass
			
	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	printf("----Testing whoseTurn function----\n\n");

	//Test initialized game
	printf("Game initialized with 2 players. Turn should be 0.\n");
	assertInts(whoseTurn(&G), 0, &testFails);

	//Set player turn to 1
	G.whoseTurn = 1;
	
	printf("Set turn to 1. Turn should be 1.\n");
	assertInts(whoseTurn(&G), 1, &testFails);
	
	//Set player turn to -1
	G.whoseTurn = -1;
	
	printf("Set turn to -1. Turn should be -1.\n");
	assertInts(whoseTurn(&G), -1, &testFails);
	
	
	if (testFails == 0)
	{
		printf("TESTING COMPLETE. ALL TESTS PASSED.\n\n");
	}
	else
	{
		printf("TESTING COMPLETE. %d FAILED\n\n", testFails);
	}
	


	return 0;
}

//Verifies a and b are the same. 
//Prints a message based on the comparison
//If a and b are not the same, count is increased by 1
void assertInts(int a, int b, int *count)
{
	if (a == b)
	{
		printf("OK\n\n");
	}
	else
	{
		printf("TEST FAILED!\n\n");
		(*count)++;
	}
}
