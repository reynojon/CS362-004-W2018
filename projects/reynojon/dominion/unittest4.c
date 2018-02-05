/*
 * unittest4.c
 *
 * Tests the functionality of the numHandCards function by checking multiple values
 * 
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

	printf("\n----Testing numHandCards function----\n\n");

	//Test initialized game
	printf("Game initialized with 2 players. Player 0 should have 5 cards.\n");
	assertInts(numHandCards(&G), 5, &testFails);

	//Check other player, no cards since game has not had player draw yet
	G.whoseTurn = 1;
	printf("Game initialized with 2 players. Player 1 should have 0 cards.\n");
	assertInts(numHandCards(&G), 0, &testFails);

	//Set player 1 card count
	G.handCount[1] = 4;
	printf("Player 1 should have 4 cards.\n");
	assertInts(numHandCards(&G), 4, &testFails);

	//Set player 1 card count
	G.handCount[1] = -1;
	printf("Player 1 should have -1 cards.\n");
	assertInts(numHandCards(&G), -1, &testFails);

	//Change turn, set player 0 card count
	G.whoseTurn = 0;
	G.handCount[0] = 99;
	
	printf("Player 0 should have 99 cards.\n");
	assertInts(numHandCards(&G), 99, &testFails);
	
	
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
