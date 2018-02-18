/*
 * randomtestadventurer.c
 *
 * Tests the functionality of the playAdventurer function
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <stdio.h>

#define NUM_ITERATIONS 3
#define DEBUGGING 1
void assertInts(int a, int b, int *count);

int main() {
  //Create game
  struct gameState G;
  
  int randSeed;
  
  // Seed Random Number Generator
  srand(time(0));
  
  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
           sea_hag, tribute, smithy}; //These do no matter, but values are require to use initializeGame()

  initializeGame(2, k, 1, &G);  //Seed does not matter as deck and discard will be manipulated

  int firstCoin = -1, secondCoin = -1;  //Next 2 coins in deck (not discard)
  int firstDraw, secondDraw, i, randHandPos;
  int failCount = 0, passCount = 0;  //Tracks the number of tests passed and failed
  int testFailed = 0;   //Used as a boolean, must be either 0 or 1;
  int initialCardCount, initialScore, initialDeckCount, initialDiscardCount, initialHandCount, initialPlayedCount;
  int tempDeck[MAX_HAND];

  printf("\n----Testing playAdventurer----\n");

  for(i = 0; i < NUM_ITERATIONS; i++)
   {
    
      printf("-----ITERATION %d-----\n", i + 1);
    
      //Reset values for iteration
      tempDeck[0] = -1; //Set initial sentinel value
      testFailed = 0;
      G.playedCount = 0;
    
      minDiscardSize = 0;
      maxDiscardSize = rand() % MAX_HAND;
    
      minDeckSize = 0;
      maxDeckSize = MAX_HAND - maxDiscardSize; //Ensure total deck + discard is less than 500
    
      //Generate discard and deck piles of random sizes containing random cards
      createRandomPiles(&G, minDiscardSize, maxDiscardSize, minDeckSize, maxDeckSize);
      
      //Put an adventurer card in hand at a random position
      randHandPos = rand() % G.handCount[G.whoseTurn];
      G.hand[G.whoseTurn][randHandPos] = adventurer;
    
      //Record values prior to playing Adventurer card
      initialCardCount = G.handCount[G.whoseTurn] + G.discardCount[G.whoseTurn] + G.deckCount[G.whoseTurn];
      initialScore = scoreFor(state->whoseTurn, state);
      initialDeckCount = G.deckCount[G.whoseTurn];
      initialDiscardCount = G.discardCount[G.whoseTurn];
      initialHandCount = G.handCount[G.whoseTurn];
          
      //Check draw pile to find the coins which should be added to the hand (if they are in the pile)
      findCoins(tempDeck, MAX_HAND - 1, firstCoin, secondCoin, &G);

      //If 1 or fewer coins in draw pile, count all coins in discard pile
      if(secondCoin == -1)
      {
        countDiscardCoins(copperCount, silverCount, goldCount, &G);
        if((copperCount + silverCount + goldCount) == 1)
        {
          if(firstCoin == -1) //Insufficient coins in the deck + discard
          {
            printf("WARNING: Only 1 total coin available.\n")
          }
        }
        else if ((copperCount + silverCount + goldCount) == 0)
        {
          printf("WARNING: No coins available.\n");
        }
      }

      playAdventurer(&G, G.whoseTurn);  //Playing the card directly bypasses end of turn actions and any check for the card's existence

      //AFTER PLAYING ADVENTURER

      //Player should have 2 more coins in hand (possibly undeterminable ahead of time)
      firstDraw = G.hand[G.whoseTurn][G.handCount[G.whoseTurn]-1];  //top card of hand is first card drawn.
      secondDraw = G.hand[G.whoseTurn][randHandPos]; //last card drawn should be at adventurer's previous position
    
      //Verify cards added to hand were actually coins and also available in the deck prior to draw
      assertCoinDraw(firstDraw, secondDraw, firstCoin, secondCoin, copperCount, silverCount, goldCount, testFailed);

      //Verify the total number of cards available to the player did not change
      assertCardCount(initialCardCount, &G, testFailed);

      //Verify total score unchanged
      assertScore(initialScore, &G, testFailed);

      //Verify cards in player's deck changed by correct amount (if it can be determined)
      if(secondCoin != -1)
      {
        i = 0;
        while(tempDeck[i] != -1)
        {
          i++;
        }
        assertDeckSize(initialDeckCount - (2 + i), &G, testFailed);
      }

      //Verify cards in discard changed by correct amount (if it can be determined)
      if(secondCoin != -1)
      {
        assertDiscardSize(initialDiscardCount + i, &G, testFailed); //i determined in deckSize verification
      }

      //Verify hand size increased by 1 (+2 coins, -1 card put in played pile)
      assertHandSize(initialHandCount + 2, &G, testFailed);
    
      //Verify adventurer card put on top of played pile and playedCount was incremented
      assertPlayed(1, &G, testFailed);

      failCount += testFailed;  //Adds 1 if any of the validations failed on this iteration    

      printf("\n-----ITERATION %d COMPLETE -----\n", i + 1);
  }
  
  printf("\n\n-----TESTING COMPLETE-----\n");
  printf("TOTAL TESTS: %d\nFAILED TESTS: %d\n", i, failCount);
  
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
void findCoins(int *nonCoinsArray, int nonCoinsCap, int &coin1, int &coin2, struct gameState *state)
{
  int i;
  int nonCoinsIndex = 0;
  coin1 = -1;
  coin2 = -1;
  int nextCard;
  
  for(i = 0; ((i < nonCoinsCap) && (coin2 == -1)); i++)
  {
    nextCard = state->deck[state->whoseTurn][i];
    if ((nextCard == copper) || (nextCard == silver) || (nextCard == gold))
    {
      if (coin1 == -1)
      {
        coin1 = nextCard;
      }
      else
      {
        coin2 = nextCard;
      }
    }
    else //Not a coin. Will be put in discard
    {
      nonCoinsArray[nonCoinsIndex] = nextCard;
      nonCoinsIndex++;
    }
  }
  nonCoinsArray[nonCoinsIndex] = -1; //Sentinel Value for end of array
}

//Counts the number of coins of each type in the discard pile for the current player
// Does not change the game state
void countDiscardCoins(int &copperCount, int &silverCount, int &goldCount, struct gameState *state)
{
  int i, tempCard;
  copperCount = 0;
  silverCount = 0;
  goldCount = 0;
  
  //Count number of each coin type in discard pile
  for(i = 0; i < state->discardCount[state->whoseTurn]; i++)
  {
    tempCard = state->discard[state->whoseTurn][i];
    if(tempCard == copper)
    {
      copperCount++;
    }
    else if (tempCard == silver)
    {
      silverCount++;
    }
    else if (tempCard == gold)
    {
      goldCount++;
    }
  }
}

//Checks if firstDraw and secondDraw are valid cards to have drawn using the Adventurer
// If the paremeters are not coins which would have been possible to draw, a failure message is printed
// If any of the checks fail, testFailed is set to 1 (true)
// If DEBUGGING is 1, a pass message will be printed if the test passes
void assertCoinDraw(int firstDraw, int secondDraw, int firstCoin, int secondCoin, int copperCount, 
                          int silverCount, int goldCount, int &testFailed)
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
          printf("-----FAILURE: First card drawn from Adventurer (after a shuffle) was not a coin!\n");
          testFailed = 1;
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
          printf("-----FAILURE: Second card drawn from Adventurer was not a coin!\n");
          testFailed = 1;
        }
      }
    if(copperCount < 0)
    {
          printf("-----FAILURE: Copper(s) added to hand which did not exist in deck\n");
          testFailed = 1;
    }
    if(silverCount < 0)
    {
          printf("-----FAILURE: Silver(s) added to hand which did not exist in deck\n");
          testFailed = 1;
    }
    if(goldCount < 0)
    {
          printf("-----FAILURE: Gold(s) added to hand which did not exist in deck\n");
          testFailed = 1;
    }
  }
  else //Cards added to hand were from the deck and no shuffle occurred
  {
    if (firstDraw != firstCoin)
    {
      printf("-----FAILURE: First card added to hand did not match next coin in deck!\n");
      testFailed = 1;
    }
    if (secondDraw != secondCoin)
    {
      printf("-----FAILURE: Copper added to hand which did not exist in deck\n");
      testFailed = 1;
    }
  }
  if(DEBUGGING && !testFailed)
  {
    printf("----PASS: Coins drawn correctly.\n");
  }
}

//Checks if prevDeckCount is equal to the current player's total number of cards
//Prints a message if the check fails
// If DEBUGGING is 1, a pass message will be printed if the check passes
//If the check fails, testFailed is set to 1 (true)
// Does not change the game state
void assertCardCount(int prevDeckCount, struct gameState *state, int &testFailed)
{
  int newCount;
  newCount = state->handCount[state->whoseTurn] + state->discardCount[state->whoseTurn] + state->deckCount[state->whoseTurn];
  
  if(prevDeckCount != newDeckCount)
  {
    printf("-----FAILURE: Total number of cards in player's deck, discard, and hand have changed!\n");
    testFailed = 1;
  }
  else if (DEBUGGING)
  {
    printf("----PASS: Total number of cards for player did not change!\n");
  }
}

//Checks if prevScore is equal to the current player's score
//Prints a message if the check fails
// If DEBUGGING is 1, a pass message will be printed if the check passes
//If the check fails, testFailed is set to 1 (true)
// Does not change the game state
void  assertScore(int prevScore, struct gameState *state, int &testFailed)
{
  int newScore;
  newScore = scoreFor(state->whoseTurn, state);
  
  if(prevScore != newScore)
  {
    printf("-----FAILURE: Score for player changed!\n");
    testFailed = 1;
  }
  else if (DEBUGGING)
  {
    printf("----PASS: Player's score did not change!\n");
  }
}

//Checks if expectedDeckSize is equal to the current player's deckCount
//Prints a message if the check fails
// If DEBUGGING is 1, a pass message will be printed if the check passes
//If the check fails, testFailed is set to 1 (true)
// Does not change the game state
void  assertDeckSize(int expectedDeckSize, struct gameState *state, int &testFailed)
{  
  if(expectedDeckSize != state->deckCount[state->whoseTurn])
  {
    printf("-----FAILURE: Player's deck did not change by correct number of cards!\n");
    testFailed = 1;
  }  
  else if (DEBUGGING)
  {
    printf("----PASS: Player's deck size changed by correct number of cards!\n");
  }
}

//Checks if expectedDiscardSize is equal to the current player's discardCount
//Prints a message if the check fails
// If DEBUGGING is 1, a pass message will be printed if the check passes
//If the check fails, testFailed is set to 1 (true)
// Does not change the game state
void  assertDiscardSize(int expectedDiscardSize, struct gameState *state, int &testFailed)
{  
  if(expectedDiscardSize != state->discardCount[state->whoseTurn])
  {
    printf("-----FAILURE: Player's discard pile did not change by correct number of cards!\n");
    testFailed = 1;
  }  
  else if (DEBUGGING)
  {
    printf("----PASS: Player's discard size changed by correct number of cards!\n");
  }
}


//Checks if expectedHandSize is equal to the current player's hand size
//Prints a message if the check fails
// If DEBUGGING is 1, a pass message will be printed if the check passes
//If the check fails, testFailed is set to 1 (true)
// Does not change the game state
void  assertHandSize(int expectedHandSize, struct gameState *state, int &testFailed)
{  
  if(expectedHandSize != state->handCount[state->whoseTurn])
  {
    printf("-----FAILURE: Player's hand size did not change by correct number of cards!\n");
    testFailed = 1;
  }  
  else if (DEBUGGING)
  {
    printf("----PASS: Player's hand size changed by correct number of cards!\n");
  }
}

//Checks if playedCount is incremented and that the adventurer card is on top of the played pile
//Prints a message if the check fails
// If DEBUGGING is 1, a pass message will be printed if the check passes
//If the check fails, testFailed is set to 1 (true)
// Does not change the game state
void  assertPlayed(int expectedPlayedCount, struct gameState *state, int &testFailed)
{  
  if((expectedPlayedCount != state->playedCount) || (state->played[state->playedCount - 1] != adventurer))
  {
    printf("-----FAILURE: Adventurer not properly added to played pile!\n");
    testFailed = 1;
  }  
  else if (DEBUGGING)
  {
    printf("----PASS: Adventurer correctly added to played pile!\n");
  }
}
