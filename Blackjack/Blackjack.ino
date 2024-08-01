#include <SPI.h> 
#include <MFRC522.h> 


#define SS_PIN 10 // slave select pin for rfid reader 
#define RST_PIN 9 // reset pin for rfid reader 
MFRC522 rfid(SS_PIN, RST_PIN); // create rfid reader object 

// unique ids for each player 
byte player1UID[] = {0xDE, 0xAD, 0xBE, 0xEF};  // replace with actual uid of player 1 
byte player2UID[] = {0xFE, 0xED, 0xFA, 0xCE};  // replace with actual uid of player 2 
byte player3UID[] = {0xBA, 0xDC, 0x0F, 0xFE};  // replace with actual uid of player 3 

// variables to hold the cards and game states 
int cards[48]; 
int numCards = 48; 

// variables for each player's hand 
int player1Hand = 0, player2Hand = 0, player3Hand = 0; 
// variables to control player actions 
bool player1Drawing = true, player2Drawing = true, player3Drawing = true; 
// variables to check if players went bust 
bool player1Bust = false, player2Bust = false, player3Bust = false; 
// boolean to control the game state 
bool gamePlaying = false; 
bool waitingForRestart = false; 



void setup() { 
  Serial.begin(9600); // start serial communication at 9600 baud rate 
  SPI.begin(); // start spi communication 
  rfid.PCD_Init(); // initialize rfid reader 
  initializeCards(); // initialize the cards array 
  randomSeed(analogRead(0)); // initialize random seed 
}


void loop(){
  if (gamePlaying) { 
      // handle game logic 
    if (player1Drawing) { 
      playerTurn(1, player1Hand, player1Drawing, player1Bust); 
    } else if (player2Drawing) {
      playerTurn(2, player2Hand, player2Drawing, player2Bust); 
    } else if (player3Drawing) { 
      playerTurn(3, player3Hand, player3Drawing, player3Bust); 
    } else { 
      determineWinner(); // determine the winner when all players are done 
      gamePlaying = false; // end game 
      waitingForRestart = true; // wait for restart input 
    } 
  } else if (waitingForRestart) { 
    // check for restart input 
    Serial.println("would you like to play again? (1 = yes, 0 = no)"); 
    while (Serial.available() == 0) { 
    // wait for input 
    } 
    int userInput = Serial.parseInt(); 
    if (userInput == 1) { 
      resetGame(); // restart the game 
    } else if (userInput == 0) { 
      Serial.println("game over."); 
      while (true); // stop execution 
    } else { 
      Serial.println("invalid input. please enter 1 to restart or 0 to end."); 
    } 
  } else { 
    // wait for player identification to start the game 
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) { 
      // check if the detected uid matches any player's uid 
      if (isSameUID(rfid.uid.uidByte, player1UID)) { 
        Serial.println("player 1 identified"); 
        player1Hand = 0; // reset player 1 hand 
        player1Drawing = true; // allow player 1 to draw 
      } else if (isSameUID(rfid.uid.uidByte, player2UID)) { 
        Serial.println("player 2 identified"); 
        player2Hand = 0; // reset player 2 hand 
        player2Drawing = true; // allow player 2 to draw 
      } else if (isSameUID(rfid.uid.uidByte, player3UID)) { 
        Serial.println("player 3 identified"); 
        player3Hand = 0; // reset player 3 hand 
        player3Drawing = true; // allow player 3 to draw 
      } else { 
        Serial.println("unknown player"); 
      } 
      rfid.PICC_HaltA(); // halt rfid card detection 
      rfid.PCD_StopCrypto1(); // stop encryption on rfid reader 

      if (!gamePlaying) { 
        gamePlaying = true; // start the game 
        dealInitialCards(); // deal initial cards to players 
      }
    } 
  } 
}


void initializeCards() { 
  // initialize the cards array with card values 
  int initialCards[48] = {2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 9, 9, 9, 9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10}; 

  for (int i = 0; i < 48; i++) { 
  cards[i] = initialCards[i]; 
  } 
  numCards = 48; // reset the number of cards 
}


void dealInitialCards() { 
  // deal initial two cards to each player 
  for (int i = 0; i < 2; i++) { 
  player1Hand += drawCard(); 
  player2Hand += drawCard(); 
  player3Hand += drawCard(); 
  } 

  // print initial hands to the serial monitor
  Serial.print("starting hand for player 1: "); 
  Serial.println(player1Hand); 
  Serial.print("starting hand for player 2: "); 
  Serial.println(player2Hand); 
  Serial.print("starting hand for player 3: "); 
  Serial.println(player3Hand); 
}

int drawCard() { 
  // draw a card from the deck 
  if (numCards == 0) { 
    Serial.println("no more cards to draw"); 
    return 0; 
  } 

  int index = random(numCards); // get random index for card 
  int cardValue = cards[index]; // get card value 
  // remove the drawn card from the array 
  for (int i = index; i < numCards - 1; i++) { 
    cards[i] = cards[i + 1]; 
  } 
  numCards--; // reduce the number of available cards 
  return cardValue; 
} 

void playerTurn(int player, int& hand, bool& drawing, bool& bust){
  // manage player's turn
  if (hand <= 21) { 

      Serial.print("player "); 
      Serial.print(player); 
      Serial.println(" would you like to draw? (1 = yes, 0 = no)");  

      while (Serial.available() == 0) { 
      // wait for input 
      } 
      int userInput = Serial.parseInt(); 

      if (userInput == 1) { 
        hand += drawCard(); // add drawn card to player's hand 
        Serial.print("player "); 
        Serial.print(player); 
        Serial.println(hand); 
        } else if (userInput == 0) { 
          drawing = false; // stop drawing if player chooses not to draw 
        }  
        else { 
          Serial.println("invalid input"); 
        } 
      } else { 
  Serial.print("player "); 
  Serial.print(player); 
  Serial.println(" went bust"); 
  drawing = false; // stop drawing if player goes bust 
  bust = true; // mark player as bust 
  } 
}

void determineWinner(){
  // determine the winner based on hand values and bust status
  if (player1Hand <= 21 && (player1Hand > player2Hand && player1Hand > player3Hand || !player1Bust && player2Bust && player3Bust)) { 
    Serial.println("player 1 wins"); 
  } else if (player2Hand <= 21 && (player2Hand > player1Hand && player2Hand > player3Hand || player1Bust && !player2Bust && player3Bust)) { 
    Serial.println("player 2 wins"); 
  } else if (player3Hand <= 21 && (player3Hand > player1Hand && player3Hand > player2Hand || player1Bust && player2Bust && !player3Bust)) { 
    Serial.println("player 3 wins"); 
  } else { 
    Serial.println("it's a draw"); 
  } 
}


bool isSameUID(byte *uid1, byte *uid2) { 
// compare two uids to check if they are the same 
for (byte i = 0; i < 4; i++) {
if (uid1[i] != uid2[i]) return false;
}



void resetGame(){
  // reset game state for a new round
  
}