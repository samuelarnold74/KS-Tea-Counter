//************************************************************************************
// KS-Tea-Counter
//************************************************************************************
// Set the counter to the time your tea should brew and start it.
// After the set time the blue LED will be turned on. After the waitingTimeOne the
// red LED will be turned on. After the waitingTimeTwo the blue led will be activated.
// Then both leds start to blink first slowly then fast - at the end the ringer is for
// the ringerTime active.
// Automatic counter reset to 0 after ringer has been activated.
//************************************************************************************
// Configuration variables
//************************************************************************************
// The time after which the red led is turned on
int waitingTimeOne = 3000;

// The time after which the blinker will be activated
int waitingTimeTwo = 4000;

// Time the ringer will be acitve
int ringerTime = 500;

//************************************************************************************
// LIBRARIES, DEFINES, INITIALIZATIONS
//************************************************************************************
// Library for OLED-Display
#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
// I2C - Address of OLED Display
#define I2C_ADDRESS 0x3C
// Create oled
SSD1306AsciiWire oled;

//************************************************************************************
// Global variables
//************************************************************************************
// After this time the LED's/Ringer get activated
int secondsToGo = 0;

// Check if countdown started
boolean countDownStarted = false;

//************************************************************************************
// Inputs and ouputs
//************************************************************************************
// Output definitions
int ringer = 2;
int blueLed = 3;
int redLed = 4;

// Input definitions
int upButton = 5; // (increase)
int downButton = 6; // (decrease)
int startStopButton = 7; // (start-reset)

//************************************************************************************
// General setup
//************************************************************************************
void setup()
{
  // Give the OLED a little bit of time
  delay(3000);
  //**********************************************************************************
  // OLED Display
  //**********************************************************************************
  Wire.begin();
  Wire.setClock(400000L);
  oled.begin(&Adafruit128x32, I2C_ADDRESS); // no reset pin
  oled.setFont(Adafruit5x7);
  oled.clear();
  writeOledDisplay("-");

  //**********************************************************************************
  // DEFINE PIN TYPES
  //**********************************************************************************
  // define the output pins
  pinMode(ringer, OUTPUT);
  pinMode(blueLed, OUTPUT);
  pinMode(redLed, OUTPUT);

  // define the input pins
  pinMode(upButton, INPUT);
  pinMode(downButton, INPUT);
  pinMode(startStopButton, INPUT);

  //**********************************************************************************
  // INITIALIZE PINS
  //**********************************************************************************
  // initialize the output pins
  digitalWrite(ringer, LOW);
  digitalWrite(blueLed, LOW);
  digitalWrite(redLed, LOW);

  // activate pull-up Resistors for switches
  digitalWrite(upButton, HIGH);
  digitalWrite(downButton, HIGH);
  digitalWrite(startStopButton, HIGH);
}
//************************************************************************************
// Main program loop
//************************************************************************************
void loop()
{
  //**********************************************************************************
  // Up button
  //**********************************************************************************
  if (digitalRead(upButton) == LOW)
  {
    // avoid bouncing
    delay(50);
    if (digitalRead(upButton) == LOW)
    {
      secondsToGo = secondsToGo + 60;
      writeOledDisplay(String(secondsToGo));
      delay(500);
    }
  }
  //**********************************************************************************
  // Down button
  //**********************************************************************************
  if (digitalRead(downButton) == LOW)
  {
    // avoid bouncing
    delay(50);
    if (digitalRead(downButton) == LOW)
    {
      secondsToGo = secondsToGo - 60;
      writeOledDisplay(String(secondsToGo));
      delay(500);
    }
  }
  //**********************************************************************************
  // Start button
  //**********************************************************************************
  if (digitalRead(startStopButton) == LOW)
  {
    // avoid bouncing
    delay(50);
    if (digitalRead(startStopButton) == LOW)
    {
      countDownStarted = true;
      for (int x = (secondsToGo-1); x > -1; x--)
      {
        delay(1000);
        writeOledDisplay(String(x));
      }
      digitalWrite(blueLed, HIGH);
      delay(waitingTimeOne);
      digitalWrite(redLed, HIGH);
      delay(waitingTimeTwo);
      for (int y=0; y<7; y++)
      {
       // Slow blinking
       digitalWrite(blueLed, LOW);
       digitalWrite(redLed, HIGH);
       delay(300);
       digitalWrite(blueLed, HIGH); 
       digitalWrite(redLed, LOW);
       delay(300);
      }
      for (int y=0; y<100; y++)
      {
       // Fast blinking
       digitalWrite(blueLed, LOW);
       digitalWrite(redLed, HIGH);
       delay(30);
       digitalWrite(blueLed, HIGH); 
       digitalWrite(redLed, LOW);
       delay(30);
      }
      // ringer
      tone(ringer,200);
      delay(ringerTime);
      noTone(ringer);
      digitalWrite(blueLed, LOW);
      digitalWrite(redLed, LOW);
      secondsToGo = 0;
      writeOledDisplay(String(secondsToGo));
    }
  }
}

void writeOledDisplay(String lineOne)
{
  // Actualize the Panel
  oled.clear();
  oled.println(lineOne);
}

//************************************************************************************
