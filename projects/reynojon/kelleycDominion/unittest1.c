/*
 * unittest1.c
 *
 * Tests the functionality of the getCost function by doing an exhaustive test of 
 * all cards set up in the game.
 * 
 * Costs verified at http://wiki.dominionstrategy.com/index.php/List_of_cards
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <stdio.h>

void assertInts(int a, int b, int *count);

int main() {
	int testFails = 0; //Counts the number of failed tests. Will remain 0 if all tests pass
	
	printf("\n----Testing getCost function----\n\n");
	
	printf("Verifying cost of curse\n");
	assertInts(getCost(curse), 0, &testFails);
	
	printf("Verifying cost of estate\n");
	assertInts(getCost(estate), 2, &testFails);
	
	printf("Verifying cost of duchy\n");
	assertInts(getCost(duchy), 5, &testFails);
	
	printf("Verifying cost of province\n");
	assertInts(getCost(province), 8, &testFails);
	
	printf("Verifying cost of copper\n");
	assertInts(getCost(copper), 0, &testFails);
	
	printf("Verifying cost of silver\n");
	assertInts(getCost(silver), 3, &testFails);
	
	printf("Verifying cost of gold\n");
	assertInts(getCost(gold), 6, &testFails);
	
	printf("Verifying cost of adventurer\n");
	assertInts(getCost(adventurer), 6, &testFails);
	
	printf("Verifying cost of council_room\n");
	assertInts(getCost(council_room), 5, &testFails);
	
	printf("Verifying cost of feast\n");
	assertInts(getCost(feast), 4, &testFails);
	
	printf("Verifying cost of gardens\n");
	assertInts(getCost(gardens), 4, &testFails);
	
	printf("Verifying cost of mine\n");
	assertInts(getCost(mine), 5, &testFails);
	
	printf("Verifying cost of remodel\n");
	assertInts(getCost(remodel), 4, &testFails);
	
	printf("Verifying cost of smithy\n");
	assertInts(getCost(smithy), 4, &testFails);
	
	printf("Verifying cost of village\n");
	assertInts(getCost(village), 3, &testFails);
	
	printf("Verifying cost of baron\n");
	assertInts(getCost(baron), 4, &testFails);
	
	printf("Verifying cost of great_hall\n");
	assertInts(getCost(great_hall), 3, &testFails);
	
	printf("Verifying cost of minion\n");
	assertInts(getCost(minion), 5, &testFails);
	
	printf("Verifying cost of steward\n");
	assertInts(getCost(steward), 3, &testFails);
	
	printf("Verifying cost of tribute\n");
	assertInts(getCost(tribute), 5, &testFails);
	
 	printf("Verifying cost of ambassador\n");
	assertInts(getCost(ambassador), 3, &testFails);

	printf("Verifying cost of cutpurse\n");
	assertInts(getCost(cutpurse), 4, &testFails);
	
	printf("Verifying cost of embargo\n");
	assertInts(getCost(embargo), 2, &testFails);
	
	printf("Verifying cost of outpost\n");
	assertInts(getCost(outpost), 5, &testFails);
	
	printf("Verifying cost of salvager\n");
	assertInts(getCost(salvager), 4, &testFails);
	
	printf("Verifying cost of sea_hag\n");
	assertInts(getCost(sea_hag), 4, &testFails);
	
	printf("Verifying cost of treasure_map\n");
	assertInts(getCost(treasure_map), 4, &testFails);
	
	printf("Verifying cost of -1\n");
	assertInts(getCost(-1), -1, &testFails);

	printf("Verifying cost of card after treasure_map\n");
	assertInts(getCost(treasure_map + 1), -1, &testFails);
	
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
