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