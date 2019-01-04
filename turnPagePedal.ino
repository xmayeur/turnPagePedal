#include "Arduino.h"
#include <DFBLE.h>

#define leftBPpin 2
#define rightBPpin 3
#define LED 13
#define durationStep 500

void setup() {
  // initialize Serial:
  Serial.begin(112500);

  pinMode(leftBPpin, INPUT_PULLUP); // connect the push button between Digital pin 2 and the ground - no pull-up resistor needed
  pinMode(rightBPpin, INPUT_PULLUP);
  pinMode(LED, OUTPUT);

  delay(200);
}
void loop() {
  int leftBP; // left button state
  int rightBP; // right button state
  long startLeft, endLeft; // time in ms - measure how long left button is maintained pressed
  long startRight, endRight; // same for right button
  int lcounter, rcounter;
  String state, prevState;
  long duration;
  bool adminMode;

  prevState = "HH";
  lcounter = 0;
  rcounter = 0;
  adminMode = false;
  duration = 1000;

  while (1)
  {
    // read both button state from digital inputs
    leftBP = digitalRead(leftBPpin);
    rightBP = digitalRead(rightBPpin);

    // establish current button state
    if (leftBP == HIGH and rightBP == HIGH)
    {
      state = "HH";
    } else if (leftBP == HIGH and rightBP == LOW)
    {
      state = "HL";
    } else if (leftBP == LOW and rightBP == HIGH)
    {
      state = "LH";
    } else
    {
      state = "LL";
    }

    // manage button state changes
    if (prevState == "HH" and state == "HL") // right button is pressed
    {
      startRight = millis();
      digitalWrite(LED, HIGH);

      if (adminMode) 
      {
        Serial.print("AT+ROLE=ROLE_PERIPHERAL") + Serial.println(); delay(100);
        Serial.print("AT+FSM=FSM_HID_USB_COM_BLE_AT") + Serial.println(); delay(100);      
        // blinkLED(250, 4);
      }
    }

    if (prevState == "HH" and state == "LH") // left button is pressed
    {
      startRight = millis();
      digitalWrite(LED, HIGH);
      if (adminMode)
      {
        Serial.print("AT+ROLE=ROLE_CENTRAL") + Serial.println(); delay(100);
        Serial.print("AT+FSM=FSM_TRANS_USB_COM_BLE") + Serial.println(); delay(100);
        // blinkLED(500, 2);        
      }
    }

    // right button is released alone
    if (prevState == "HL" and state == "HH")
    {
      endRight = millis(); // stop right button timer
      if (adminMode) // exit from admin mode if it was set
      {
        adminMode = false;
      } else if (lcounter > 0) // manage long press parameter
      {
        duration = lcounter * durationStep;
        lcounter = 0;
      } else if (endRight - startRight > duration) // long press > duration ms will send the 'c' character
      {
        BLE.press_key(HID_KEYBOARD_C); delay(100);
        BLE.press_key(HID_KEYBOARD_RESERVED); delay(10);
      } else // right button short press - send a right cursor code
      {
        digitalWrite(LED, LOW);
        BLE.press_key(HID_KEYBOARD_RIGHT_ARROW); delay(100); // short press sends a right cursor code
        BLE.press_key(HID_KEYBOARD_RESERVED); delay(10);
      }
    }

    // left button is released alone
    if (prevState == "LH" and state == "HH") // left button is released
    {
      endLeft = millis(); // stop left button timer
      if (adminMode) // exit from admin mode if it was set
      {
        adminMode = false;
      } else if (rcounter >= 3) // enter admin mode
      {
        adminMode = true;
        rcounter = 0;
      } else if (endRight - startRight > duration) // long press > duration ms  - nothing for now
      {
        // do something later
      } else // left button short press - send a left cursor code
      {
        digitalWrite(LED, LOW);
        BLE.press_key(HID_KEYBOARD_LEFT_ARROW); delay(100); // short press sends a left cursor code
        BLE.press_key(HID_KEYBOARD_RESERVED); delay(10);
      }
    }

    // right button is maintained down - left button is pressed
    if (prevState == "HL" and state == "LL")
    {
      lcounter = (++lcounter) % 5; // increase counter for long press duration setting
    }

    // left button is maintained down - right button is pressed
    if (prevState == "LH" and state == "LL")
    {
      rcounter += 1; // increase counter to pass in admin mode
    }

    // memorize previous state
    prevState = state;
    delay(100);
  } // end of while(1)
}

void blinkLED(int period, int nr)
{
  int i;
  for (i = 1; i <= nr; 1)
  {
    digitalWrite(LED, HIGH);
    delay(period);
    digitalWrite(LED, LOW);
    delay(period);
  }
}

