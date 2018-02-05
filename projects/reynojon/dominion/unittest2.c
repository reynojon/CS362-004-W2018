/*
 * unittest2.c
 *
 * Tests the functionality of the isGameOver function by checking multiple paths and branches.
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

	printf("\n----Testing isGameOver function----\n\n");

	//Test initialized game
	printf("Game initialized. Game should not be over.\n");
	assertInts(isGameOver(&G), 0, &testFails);

	//Set province count to 0
	G.supplyCount[province] = 0;
	
	printf("Province deck empty. Game should be over.\n");
	assertInts(isGameOver(&G), 1, &testFails);
	
	//Set province count to -1. The game should technically never reach this state, as province cards are required
	G.supplyCount[province] = -1;
	
	printf("Province deck has count of -1. Game should not be over.\n");
	assertInts(isGameOver(&G), 0, &testFails);
	
	//Set province count to 1 and 1 supply to empty
	G.supplyCount[province] = 1;
	G.supplyCount[curse] = 0;
	
	printf("Province deck not empty. 1 supply deck empty. Game should not be over.\n");
	assertInts(isGameOver(&G), 0, &testFails);
	
	//Make second supply empty
	G.supplyCount[duchy] = 0;

	printf("Province deck not empty. 2 supply decks empty. Game should not be over.\n");
	assertInts(isGameOver(&G), 0, &testFails);
	
	//Make third supply empty
	G.supplyCount[estate] = 0;

	printf("Province deck not empty. Curse, Duchy and Estate are empty. Game should be over.\n");
	assertInts(isGameOver(&G), 1, &testFails);
	
	//Make curse deck not empty but adventurer empty
	G.supplyCount[curse] = 1;
	
	printf("Province deck not empty. Put cards back into curse deck. Game should not be over.\n");
	assertInts(isGameOver(&G), 0, &testFails);

	//Make a deck in the middle of the enum range empty
	G.supplyCount[minion] = 0;
	
	printf("Province deck not empty. Minion, Duchy and Estate are empty. Game should be over.\n");
	assertInts(isGameOver(&G), 1, &testFails);

	//Make a deck in the end of the enum range empty. Make minion not empty
	G.supplyCount[minion] = 1;
	G.supplyCount[treasure_map] = 0;
	
	printf("Province deck not empty. Treasure Map, Duchy and Estate are empty. Game should be over.\n");
	assertInts(isGameOver(&G), 1, &testFails);
	
	//Empty Province Deck. Keep same supplies
	G.supplyCount[province] = 0;
	
	printf("Province deck empty. Treasure Map, Duchy and Estate are empty. Game should be over.\n");
	assertInts(isGameOver(&G), 1, &testFails);
	
	//10 Supplies are empty. Province deck empty.
	G.supplyCount[adventurer] = 0;
	G.supplyCount[embargo] = 0;
	G.supplyCount[village] = 0;
	G.supplyCount[minion] = 0;
	G.supplyCount[mine] = 0;
	G.supplyCount[cutpurse] = 0;
	G.supplyCount[sea_hag] = 0;
	G.supplyCount[tribute] = 0;
	G.supplyCount[smithy] = 0;
	G.supplyCount[treasure_map] = 0;
	
	printf("Province deck empty. 10 supply decks are empty. Game should be over.\n");
	assertInts(isGameOver(&G), 1, &testFails);
	
	//10 supplies are empty. Province deck not empty.
	G.supplyCount[province] = 1;
	
	printf("Province deck not empty. 10 supply decks are empty. Game should be over.\n");
	assertInts(isGameOver(&G), 1, &testFails);

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
