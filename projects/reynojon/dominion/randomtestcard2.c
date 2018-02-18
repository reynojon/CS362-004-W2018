  /*
 * randomtestcard2.c
 *
 * Tests the functionality of the playSmithy function
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

  int firstCard, secondCard, thirdCard;
  int firstDraw, secondDraw, thirdDraw, i, randHandPos;
  int failCount = 0, passCount = 0;  //Tracks the number of tests passed and failed
  int testFailed = 0;   //Used as a boolean, must be either 0 or 1;
  int initialCardCount, initialScore, initialDeckCount, initialDiscardCount, initialHandCount, initialPlayedCount;
  int tempDeck[treasure_map]; //Holds the count of each card in discard pile

  printf("\n----Testing playAdventurer----\n");

  for(i = 0; i < NUM_ITERATIONS; i++)
   {
    
      printf("-----ITERATION %d-----\n", i + 1);
    
      //Reset values for iteration
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
      G.hand[G.whoseTurn][randHandPos] = smithy;
    
      //Record values prior to playing Adventurer card
      initialCardCount = G.handCount[G.whoseTurn] + G.discardCount[G.whoseTurn] + G.deckCount[G.whoseTurn];
      initialScore = scoreFor(state->whoseTurn, state);
      initialDeckCount = G.deckCount[G.whoseTurn];
      initialDiscardCount = G.discardCount[G.whoseTurn];
      initialHandCount = G.handCount[G.whoseTurn];
          
      //Check draw pile to determine the three cards which should be drawn
      findDrawCards(firstCard, secondCard, thirdCard, &G);
    
      //If 2 or fewer cards in draw pile, record all of the cards in the discard pile
      if(thirdCard == -1)
      {
        countDiscardPile(tempDeck, treasure_map, &G);
        if(((firstCard == -1) && (G.discardCount < 3)) || ((secondCard == -1) && (G.discardCount < 2)) ||
          || (G.discardCount < 1))
        {
           printf("WARNING: Fewer than 3 cards to draw.\n")
        }
      }

      playSmithy(&G, G.whoseTurn, randHandPos);  //Playing the card directly bypasses end of turn actions and any check for the card's existence

      //AFTER PLAYING SMITHY

      //Player should have 3 new cards in hand (possibly undeterminable ahead of time)
      firstDraw = G.hand[G.whoseTurn][G.handCount[G.whoseTurn]-2];  //second from top card of hand is first card drawn.
      secondDraw = G.hand[G.whoseTurn][G.handCount[G.whoseTurn]-1]; //top card of hand is second card drawn
      thirdDraw = G.hand[G.whoseTurn][randHandPos];  //Last card drawn should be at smithy's previous position

      //Verify cards added to hand match the top cards of the deck pile, and/or were actually in the discard pile
      assertSmithyDraw(firstCard, secondCard, thirdCard, firstDraw, secondDraw, thirdDraw, tempDeck, testFailed);
    
      //Verify the total number of cards available to the player did not change
      assertCardCount(initialCardCount, &G, testFailed);

      //Verify total score unchanged
      assertScore(initialScore, &G, testFailed);

      
      if(thirdCard != -1)
      {
        assertDeckSize(initialDeckCount - 3, &G, testFailed);	//Verify cards in player's deck changed by correct amount (if not shuffled)
        assertDiscardSize(initialDiscardCount, &G, testFailed); //Verify cards in discard did not change if it was not shuffled
      }
    
      //Verify hand size increased by 2 (+3 cards, -1 card put in played pile)
      assertHandSize(initialHandCount + 2, &G, testFailed);
    
      //Verify smithy card put on top of played pile and playedCount was incremented
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

//Checks the 'top' three cards of the current player's and stores their values
// If there are fewer than three cards in the current player's deck, -1 is stored
//  in any values where a card is not present
// Does not change the game state
void findDrawCards(int &firstCard, int &secondCard, int &thirdCard, struct gameState *state)
{
  int i, deckSize;
  firstCard = -1;
  secondCard = -1;
  thirdCard = -1;
 
  deckSize = state->deckCount[state->whoseTurn];
  
  if(deckSize > 0)
  {
    firstCard = state->deck[state->whoseTurn][deckSize - 1]
  }
  if(deckSize > 1)
  {
    secondCard = state->deck[state->whoseTurn][deckSize - 2];
  }
  if(deckSize > 2)
  {
    thirdCard = state->deck[state->whoseTurn][deckSize - 3];
  }
}

//Counts the number of cards of each type in the discard pile for the current player
// Does not change the game state
void countDiscardPile(int *arr, int arrSize, struct gameState *state)
{
  int i, tempCard;
  
  //Initialize all values in arr to 0
  for(i = 0; i < arrSize; i++)
  {
    arr[i] = 0;
  }
  
  //Count number of each card type in discard pile
  for(i = 0; i < state->discardCount[state->whoseTurn]; i++)
  {
    tempCard = state->discard[state->whoseTurn][i];
    if((tempCard > treasure_map) || (tempCard < 0))
    {
      printf("WARNING: Unknown card in discard pile!\n");
    }
    else
    {
      arr[tempCard]++;  //Valid card, increment count
    }
  }
}

//Checks if the three cards drawn could have been drawn either from the deck or the discard pile if shuffled 
// If the paremeters are not cards which would have been possible to draw, a failure message is printed
// If any of the checks fail, testFailed is set to 1 (true)
// If DEBUGGING is 1, a pass message will be printed if the test passes
void assertSmithyDraw(int firstCard, int secondCard, int thirdCard, int firstDraw, int secondDraw, int thirdDraw, int *arr,
                     int &testFailed)

{
  if (firstCard == -1) //No cards in the deck. Discard was shuffled into deck
  {
    arr[firstDraw]--;
  }
  else if (firstCard != firstDraw)
  {
    printf("-----FAILURE: First card drawn from Smithy was not first card in the deck!\n");
    testFailed = 1;
  }

  if (secondCard == -1) //1 or fewer cards in the deck. Discard was shuffled into deck
  {
    arr[secondDraw]--;
  }
  else if (secondCard != secondDraw)
  {
    printf("-----FAILURE: Second card drawn from Smithy was not second card in the deck!\n");
    testFailed = 1;
  }

  if (thirdCard == -1) //2 or fewer cards in the deck. Discard was shuffled into deck
  {
    arr[thirdDraw]--;
  }
  else if (thirdCard != thirdDraw)
  {
    printf("-----FAILURE: Third card drawn from Smithy was not third card in the deck!\n");
    testFailed = 1;
  }

  if(arr[firstDraw] < 0)
  {
    printf("-----FAILURE: First card was drawn after shuffle but did not exist in discard!\n");
    testFailed = 1;
  }
  
  if(arr[secondDraw] < 0)
  {
    printf("-----FAILURE: Second card was drawn after shuffle but did not exist in discard!\n");
    testFailed = 1;
  }
  
  if(arr[thirdDraw] < 0)
  {
    printf("-----FAILURE: Third card was drawn after shuffle but did not exist in discard!\n");
    testFailed = 1;
  }
  
  if(DEBUGGING && !testFailed)
  {
    printf("----PASS: Cards drawn correctly.\n");
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
    printf("-----FAILURE: Player's discard pile size changed!\n");
    testFailed = 1;
  }  
  else if (DEBUGGING)
  {
    printf("----PASS: Player's discard size did not change!\n");
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

//Checks if playedCount is incremented and that the smithy card is on top of the played pile
//Prints a message if the check fails
// If DEBUGGING is 1, a pass message will be printed if the check passes
//If the check fails, testFailed is set to 1 (true)
// Does not change the game state
void  assertPlayed(int expectedPlayedCount, struct gameState *state, int &testFailed)
{  
  if((expectedPlayedCount != state->playedCount) || (state->played[state->playedCount - 1] != smithy))
  {
    printf("-----FAILURE: Adventurer not properly added to played pile!\n");
    testFailed = 1;
  }  
  else if (DEBUGGING)
  {
    printf("----PASS: Adventurer correctly added to played pile!\n");
  }
}
