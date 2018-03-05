/*
 * AUTHOR: reynojon
 * DATE: 2/18/2018
 * FILENAME: randomtestadventurer.c
 *
 * Tests the playAdventurer function
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_ITERATIONS 10000
#define DEBUGGING 0
#define NUM_TESTS 7 //Number of times assert functions are called per iteration

void createRandomPiles(struct gameState *state, int discardMin, int discardMax, int deckMin, int deckMax);
void findCoins(int *nonCoinsArray, int *coin1, int *coin2, struct gameState *state);
void countDiscardCoins(int *copperCount, int *silverCount, int *goldCount, struct gameState *state);
void assertCoinDraw(int firstDraw, int secondDraw, int firstCoin, int secondCoin, int copperCount, 
                          int silverCount, int goldCount, int *testFailed, int *failArray, int failIndex);
void assertCardCount(int prevDeckCount, struct gameState *state, int *testFailed, int *failArray, int failIndex);
void assertScore(int prevScore, struct gameState *state, int *testFailed, int *failArray, int failIndex);
void assertDeckSize(int expectedDeckSize, struct gameState *state, int *testFailed, int *failArray, int failIndex);
void assertDiscardSize(int expectedDiscardSize, struct gameState *state, int *testFailed, int *failArray, int failIndex);
void assertHandSize(int expectedHandSize, struct gameState *state, int *testFailed, int *failArray, int failIndex);
void assertPlayed(int expectedPlayedCount, struct gameState *state, int *testFailed, int *failArray, int failIndex);

int main() {
  // Seed Random Number Generator
  srand(time(0));
  
  //Create game
  struct gameState G;
  
  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
           sea_hag, tribute, smithy}; //These do no matter, but values are require to use initializeGame()

  initializeGame(2, k, 1, &G);  //Seed does not matter as deck and discard will be manipulated

  int firstCoin = -1, secondCoin = -1;  //Next 2 coins in deck (not discard)
  int firstDraw, secondDraw, i, j, randHandPos;
  int copperCount, silverCount, goldCount;
  int failCount[NUM_TESTS];  //Tracks the number of tests failed
  int testFailed = 0;   //Used as a boolean, must be either 0 or 1;
  int initialCardCount, initialScore, initialDeckCount, initialDiscardCount, initialHandCount;
  int minDiscardSize, maxDiscardSize, minDeckSize, maxDeckSize;
  int tempDeck[MAX_HAND];

  
  //Initialize failCountArray
  for (i = 0; i < NUM_TESTS; i++)
  {
	  failCount[i] = 0;
  }
  
  printf("\n----Testing playAdventurer----\n");

  for(i = 0; i < NUM_ITERATIONS; i++)
   {
	  if(DEBUGGING)
	  {
	  	printf("\n-----ITERATION %d-----\n", i + 1);
	  }
    
      //Reset values for iteration
      tempDeck[0] = -1; //Set initial sentinel value
      testFailed = 0;
      G.playedCardCount = 0;
	  
      minDiscardSize = 0;
      maxDiscardSize = rand() % MAX_HAND;
    
      minDeckSize = 0;
      maxDeckSize = MAX_HAND - maxDiscardSize; //Ensure total deck + discard is less than 500
    
      //Generate discard and deck piles of random sizes containing random cards
      createRandomPiles(&G, minDiscardSize, maxDiscardSize, minDeckSize, maxDeckSize);
      
      //Put an adventurer card in hand at a random position
      randHandPos = rand() % G.handCount[G.whoseTurn];
	  if(DEBUGGING)
	  {
		  printf("Adventurer card in hand position: %d\n", randHandPos);
	  }
      G.hand[G.whoseTurn][randHandPos] = adventurer;
    
      //Record values prior to playing Adventurer card
      initialCardCount = G.handCount[G.whoseTurn] + G.discardCount[G.whoseTurn] + G.deckCount[G.whoseTurn];
      initialScore = scoreFor(G.whoseTurn, &G);
      initialDeckCount = G.deckCount[G.whoseTurn];
      initialDiscardCount = G.discardCount[G.whoseTurn];
      initialHandCount = G.handCount[G.whoseTurn];
          
      //Check draw pile to find the coins which should be added to the hand (if they are in the pile)
      findCoins(tempDeck, &firstCoin, &secondCoin, &G);

      //If 1 or fewer coins in draw pile, count all coins in discard pile
      if(secondCoin == -1)
      {
        countDiscardCoins(&copperCount, &silverCount, &goldCount, &G);
        if((copperCount + silverCount + goldCount) == 1)
        {
          if(firstCoin == -1) //Insufficient coins in the deck + discard
          {
            printf("WARNING: Only 1 total coin available.\n");
          }
        }
        else if ((copperCount + silverCount + goldCount) == 0)
        {
          printf("WARNING: No coins available.\n");
        }
      }

	  /************************************************
	  * Code for my refactored adventurer
	  ************************************************/
	  //Playing the card directly bypasses end of turn actions and any check for the card's existence
      playAdventurer(&G, G.whoseTurn, randHandPos);  

      //AFTER PLAYING ADVENTURER

      //Player should have 2 more coins in hand (possibly unknown ahead of time)
      firstDraw = G.hand[G.whoseTurn][G.handCount[G.whoseTurn]-1];  //top card of hand is first card drawn.
      secondDraw = G.hand[G.whoseTurn][randHandPos]; //last card drawn should be at adventurer's previous position
    
      //Verify cards added to hand were actually coins and also available in the deck prior to draw
	  testFailed = 0;
      assertCoinDraw(firstDraw, secondDraw, firstCoin, secondCoin, copperCount, silverCount, goldCount, &testFailed, failCount, 0);
	  
	  if(DEBUGGING)
	  {
		  printf("New card in adventurer's hand position: %d\n", G.hand[G.whoseTurn][randHandPos]);
	  }
      //Verify the total number of cards available to the player did not change (except for adventurer being in played pile)
	  testFailed = 0;
      assertCardCount(initialCardCount - 1, &G, &testFailed, failCount, 1);

      //Verify total score unchanged
	  testFailed = 0;
      assertScore(initialScore, &G, &testFailed, failCount, 2);

      //Verify cards in player's deck changed by correct amount (if it can be determined)
	  testFailed = 0;
      if(secondCoin != -1)
      {
        j = 0;
        while(tempDeck[j] != -1)	//Determine number of cards revealed off deck which weren't coins
        {
          j++;
        }
        assertDeckSize(initialDeckCount - (2 + j), &G, &testFailed, failCount, 3);
      }

      //Verify cards in discard changed by correct amount (if it can be determined)
	  testFailed = 0;
      if(secondCoin != -1)
      {
        assertDiscardSize(initialDiscardCount + j, &G, &testFailed, failCount, 4); //j determined in deckSize verification
      }

      //Verify hand size increased by 1 (+2 coins, -1 card put in played pile)
	  testFailed = 0;
      assertHandSize(initialHandCount + 2, &G, &testFailed, failCount, 5);
    
      //Verify adventurer card put on top of played pile and playedCount was incremented
	  testFailed = 0;
      assertPlayed(1, &G, &testFailed, failCount, 6);

	  //End the turn to redraw 5 cards and reset player's hand variables
	  endTurn(&G);

	  if(DEBUGGING)
	  {
		printf("\n-----ITERATION %d COMPLETE -----\n", i + 1);
	  }
  }
  
  printf("-----TESTING COMPLETE-----\n");
  printf("TOTAL TESTS: %d\n", i);
  printf("assertCoinDraw failed %d times.\n", failCount[0]);
  printf("assertCardCount failed %d times.\n", failCount[1]);
  printf("assertScore failed %d times.\n", failCount[2]);
  printf("assertDeckSize failed %d times.\n", failCount[3]);
  printf("assertDiscardSize failed %d times.\n", failCount[4]);
  printf("assertHandSize failed %d times.\n", failCount[5]);
  printf("assertPlayed failed %d times.\n\n\n", failCount[6]);

	return 0;
}

//Adds random cards to the discard and deck piles for the current player
//The sizes of the two piles are random between their corresponding min and max parameters
// Changes the game state for the current player
void createRandomPiles(struct gameState *state, int discardMin, int discardMax, int deckMin, int deckMax)
{
  int numCards, cardToAdd, i;

  //Determine size of discard pile
  numCards = (rand() % (discardMax + 1)) + discardMin;
  state->discardCount[state->whoseTurn] = numCards;
  
  //Add random cards to the player's discard
  for(i = 0; i < numCards; i++)
  {
    cardToAdd = rand() % (treasure_map + 1);  //Random game card
    state->discard[state->whoseTurn][i] = cardToAdd;
  }
  
  //Determine size of deck pile
  numCards = (rand() % (deckMax + 1)) + deckMin;
  state->deckCount[state->whoseTurn] = numCards;
  
  //Add random cards to the player's deck
  for(i = 0; i < numCards; i++)
  {
    cardToAdd = rand() % (treasure_map + 1);  //Random game card
    state->deck[state->whoseTurn][i] = cardToAdd;
  }
  
}

//Checks the 'top' cards of the current player's deck in order until 2 coins are found, if they are present
// Stores the value of each non-coin in nonCoinsArray, adding a -1 after the last value is added
// Stores the coin types of the first 2 cards in the deck in coin1 and coin2
// If 2 coins are not found, -1 stored as the coin1 and/or coin2 values
// Does not change the game state
void findCoins(int *nonCoinsArray, int *coin1, int *coin2, struct gameState *state)
{
  int i;
  int nonCoinsIndex = 0;
  *coin1 = -1;
  *coin2 = -1;
  int nextCard;
  
  int currentTurn = state->whoseTurn;
  
  i = state->deckCount[currentTurn];

  while ((i >= 0) && (*coin2 == -1))
  {
    nextCard = state->deck[currentTurn][i];
    if ((nextCard == copper) || (nextCard == silver) || (nextCard == gold))
    {
      if (*coin1 == -1)
      {
        *coin1 = nextCard;
      }
      else
      {
        *coin2 = nextCard;
      }
    }
    else //Not a coin. Will be put in discard
    {
      nonCoinsArray[nonCoinsIndex] = nextCard;
      nonCoinsIndex++;
    }
	i--;
  }
  nonCoinsArray[nonCoinsIndex] = -1; //Sentinel Value for end of array

}

//Counts the number of coins of each type in the discard pile for the current player
// Does not change the game state
void countDiscardCoins(int *copperCount, int *silverCount, int *goldCount, struct gameState *state)
{
  int i, tempCard;
  *copperCount = 0;
  *silverCount = 0;
  *goldCount = 0;
  
  //Count number of each coin type in discard pile
  for(i = 0; i < state->discardCount[state->whoseTurn]; i++)
  {
    tempCard = state->discard[state->whoseTurn][i];
    if(tempCard == copper)
    {
      (*copperCount)++;
    }
    else if (tempCard == silver)
    {
      (*silverCount)++;
    }
    else if (tempCard == gold)
    {
      (*goldCount)++;
    }
  }
}

//Checks if firstDraw and secondDraw are valid cards to have drawn using the Adventurer
// If the paremeters are not coins which would have been possible to draw, a failure message is printed
// If any of the checks fail, testFailed is set to 1 (true)
// If DEBUGGING is 1, a pass message will be printed if the test passes
void assertCoinDraw(int firstDraw, int secondDraw, int firstCoin, int secondCoin, int copperCount, 
                          int silverCount, int goldCount, int *testFailed, int *failArray, int failIndex)
{
  if (secondCoin == -1) //1 or fewer coins in the deck. discard was shuffled into deck
  {
	if(firstCoin == -1) //No coins to draw from deck, discard was shuffled into deck
	//Track available quantity of each coin
	{
        if(firstDraw == copper)
        {
          copperCount--; 
        }
        else if (firstDraw == silver)
        {
          silverCount--;
        }
        else if (firstDraw == gold)
        {
          goldCount--;
        }
        else
        {
			if(DEBUGGING)
			{
				printf("-----FAILURE: First card drawn from Adventurer (after a shuffle) was not a coin!\n");
			}
          *testFailed = 1;
        }
	}
	else
	{
        if(secondDraw == copper)
        {
          copperCount--;
        }
        else if (secondDraw == silver)
        {
          silverCount--;
        }
        else if (secondDraw == gold)
        {
          goldCount--;
        }
        else
        {
			if(DEBUGGING)
			{
				printf("-----FAILURE: Second card drawn from Adventurer was not a coin!\n");
			}
			*testFailed = 1;
        }
    }
    if(copperCount < 0)
    {
		if(DEBUGGING)
		{
			printf("-----FAILURE: Copper(s) added to hand which did not exist in deck\n");
		}
        *testFailed = 1;
    }
    if(silverCount < 0)
    {
		if(DEBUGGING)
		{
			printf("-----FAILURE: Silver(s) added to hand which did not exist in deck\n");
		}
        *testFailed = 1;
    }
    if(goldCount < 0)
    {
		if(DEBUGGING)
		{
			printf("-----FAILURE: Gold(s) added to hand which did not exist in deck\n");
		}
		*testFailed = 1;
    }
  }
  else //Cards added to hand were from the deck and no shuffle occurred
  {
    if (firstDraw != firstCoin)
    {
		if(DEBUGGING)
		{
			printf("-----FAILURE: First card added to hand did not match next coin in deck!\n");
		}
		*testFailed = 1;
    }
    if (secondDraw != secondCoin)
    {
		if(DEBUGGING)
		{
			printf("-----FAILURE: Copper added to hand which did not exist in deck\n");
		}
		*testFailed = 1;
    }
  }
  if((DEBUGGING) && !(*testFailed))
  {
    printf("-----PASS: Coins drawn correctly.\n");
  }
  
  if(*testFailed)
  {
	  failArray[failIndex]++;
  }
}

//Checks if prevDeckCount is equal to the current player's total number of cards
//Prints a message if the check fails
// If DEBUGGING is 1, a pass message will be printed if the check passes
//If the check fails, testFailed is set to 1 (true)
// Does not change the game state
void assertCardCount(int prevDeckCount, struct gameState *state, int *testFailed, int *failArray, int failIndex)
{
  int newDeckCount;
  newDeckCount = state->handCount[state->whoseTurn] + state->discardCount[state->whoseTurn] + state->deckCount[state->whoseTurn];
  
  if(prevDeckCount != newDeckCount)
  {
	if(DEBUGGING)
	{
		printf("-----FAILURE: Total number of cards in player's deck, discard, and hand have changed!\n");
	}
    *testFailed = 1;
  }
  else if (DEBUGGING)
  {
	if(DEBUGGING)
	{
		printf("-----PASS: Total number of cards for player did not change!\n");
	}
  }
  
  if(*testFailed)
  {
	failArray[failIndex]++;
  }
}

//Checks if prevScore is equal to the current player's score
//Prints a message if the check fails
// If DEBUGGING is 1, a pass message will be printed if the check passes
//If the check fails, testFailed is set to 1 (true)
// Does not change the game state
void assertScore(int prevScore, struct gameState *state, int *testFailed, int *failArray, int failIndex)
{
  int newScore;
  newScore = scoreFor(state->whoseTurn, state);
  
  if(prevScore != newScore)
  {
	if(DEBUGGING)
	{
		printf("-----FAILURE: Score for player changed!\n");
	}
    *testFailed = 1;
  }
  else if (DEBUGGING)
  {
	printf("-----PASS: Player's score did not change!\n");
  }
  
  if(*testFailed)
  {
	  failArray[failIndex]++;
  }
}

//Checks if expectedDeckSize is equal to the current player's deckCount
//Prints a message if the check fails
// If DEBUGGING is 1, a pass message will be printed if the check passes
//If the check fails, testFailed is set to 1 (true)
// Does not change the game state
void assertDeckSize(int expectedDeckSize, struct gameState *state, int *testFailed, int *failArray, int failIndex)
{  
  if(expectedDeckSize != state->deckCount[state->whoseTurn])
  {
	if(DEBUGGING)
	{
		printf("-----FAILURE: Player's deck did not change by correct number of cards!\n");
	}
    *testFailed = 1;
  }  
  else if (DEBUGGING)
  {
    printf("-----PASS: Player's deck size changed by correct number of cards!\n");
  }
  
  if(*testFailed)
  {
	  failArray[failIndex]++;
  }
}

//Checks if expectedDiscardSize is equal to the current player's discardCount
//Prints a message if the check fails
// If DEBUGGING is 1, a pass message will be printed if the check passes
//If the check fails, testFailed is set to 1 (true)
// Does not change the game state
void assertDiscardSize(int expectedDiscardSize, struct gameState *state, int *testFailed, int *failArray, int failIndex)
{  
  if(expectedDiscardSize != state->discardCount[state->whoseTurn])
  {
	if(DEBUGGING)
	{
		printf("-----FAILURE: Player's discard pile did not change by correct number of cards!\n");
	}
    *testFailed = 1;
  }  
  else if (DEBUGGING)
  {
    printf("-----PASS: Player's discard size changed by correct number of cards!\n");
  }

  if(*testFailed)
  {
	  failArray[failIndex]++;
  }
}


//Checks if expectedHandSize is equal to the current player's hand size
//Prints a message if the check fails
// If DEBUGGING is 1, a pass message will be printed if the check passes
//If the check fails, testFailed is set to 1 (true)
// Does not change the game state
void  assertHandSize(int expectedHandSize, struct gameState *state, int *testFailed, int *failArray, int failIndex)
{  
  if(expectedHandSize != state->handCount[state->whoseTurn])
  {
	if(DEBUGGING)
	{
		printf("-----FAILURE: Player's hand size did not change by correct number of cards!\n");
	}
    *testFailed = 1;
  }  
  else if (DEBUGGING)
  {
    printf("-----PASS: Player's hand size changed by correct number of cards!\n");
  }
  
  if(*testFailed)
  {
	  failArray[failIndex]++;
  }
}

//Checks if playedCount is incremented and that the adventurer card is on top of the played pile
//Prints a message if the check fails
// If DEBUGGING is 1, a pass message will be printed if the check passes
//If the check fails, testFailed is set to 1 (true)
// Does not change the game state
void  assertPlayed(int expectedPlayedCount, struct gameState *state, int *testFailed, int *failArray, int failIndex)
{  
  if((expectedPlayedCount != state->playedCardCount) || (state->playedCards[state->playedCardCount - 1] != adventurer))
  {
	if(DEBUGGING)
	{
		printf("-----FAILURE: Adventurer not properly added to played pile!\n");
	}
    *testFailed = 1;
  }  
  else if (DEBUGGING)
  {
    printf("-----PASS: Adventurer correctly added to played pile!\n");
  }
  
  if(*testFailed)
  {
	  failArray[failIndex]++;
  }
}
