/*
 * cardtest1.c
 *
 * Tests the functionality of the adventurer card
 * 
 * Costs verified at http://wiki.dominionstrategy.com/index.php/List_of_cards
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <stdio.h>

void assertInts(int a, int *count);

int main() {
    int seed = 1000, numTests = 50;
    int numPlayers = 2;
	struct gameState G;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, treasure_map};
	int testFails = 0; //Will remain zero if all tests pass
	int tempHandSize, draw1, draw2;
	int i, currentPlayer;
	
	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);
	
	tempHandSize = numHandCards(&G);
	
	
	printf("\n----Testing Adventurer Card----\n\n");

	for(i = 0; i < numTests; i++)
	{
	
		playAdventurer(&G, whoseTurn(&G));
		currentPlayer = whoseTurn(&G);
		
		//Top two cards in hand should be newly drawn and also be coins
		draw2 = G.hand[currentPlayer][G.handCount[currentPlayer]-1];
		draw1 = G.hand[currentPlayer][G.handCount[currentPlayer]-2];
		
		printf("Played Adventurer Card. Top 2 cards in hand should be coins\n");	
		printf("Testing last card in hand\n");
		assertInts(draw2, &testFails);
	
		printf("Testing next-to-last card in hand\n");
		assertInts(draw1, &testFails);
		
		endTurn(&G);
	}

	if (testFails == 0)
	{
		printf("TESTING COMPLETE. %d TESTS PASSED.\n\n", numTests);
	}
	else
	{
		printf("TESTING COMPLETE. %d PASSED, %d FAILED\n\n", numTests - testFails, testFails);
	}
	
	return 0;
}

//Verifies a is either copper, silver, or gold. 
//Prints a message based on the comparison
//If a and b are not the same, count is increased by 1
void assertInts(int a, int *count)
{
	if ((a == copper) || (a == silver) || (a == gold))
	{
		printf("OK\n\n");
	}
	else
	{
		printf("TEST FAILED!\n\n");
		(*count)++;
	}
}
