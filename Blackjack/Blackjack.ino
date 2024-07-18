#include <SPI.h> 
#include <MFRC522.h> 

void setup() { 
  Serial.begin(9600); // start serial communication at 9600 baud rate 
  SPI.begin(); // start spi communication 
  rfid.PCD_Init(); // initialize rfid reader 
  initializeCards(); // initialize the cards array 
  randomSeed(analogRead(0)); // initialize random seed 
} 