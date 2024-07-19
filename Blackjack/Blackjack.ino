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