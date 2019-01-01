#include "Arduino.h"
#include <DFBLE.h>

#define leftBPpin 2
#define rightBPpin 3
#define LED 13
#define DEBUG true

void setup() {
  // initialize Serial:
  Serial.begin(112500);

  pinMode(leftBPpin, INPUT_PULLUP); // connect the push button between Digital pin 2 and the ground - no pull-up resistor needed
  pinMode(rightBPpin, INPUT_PULLUP);
  pinMode(LED, OUTPUT);

  delay(200);
}
void loop() {
  int leftBP, leftBPPrev; // left button state and previous state
  int rightBP, rightBPPrev; // right button state and previous state
  long startLeft, endLeft; // time in ms - measure how long left button is maintained pressed
  long startRight, endRight; // same for right button
  
  leftBPPrev = HIGH;
  rightBPPrev = HIGH;

  while (1)
  {
    // read both button state from digital inputs
	leftBP = digitalRead(leftBPpin);
    rightBP = digitalRead(rightBPpin);

	/// detect right button state changes
    if (rightBP == HIGH and rightBPPrev == LOW) // right button released
    {
      endRight = millis(); // stop counting time
      if (endRight - startRight > 500) // long press > 500 ms will send the 'c' character
      {
        BLE.press_key(HID_KEYBOARD_C); delay(100);
        BLE.press_key(HID_KEYBOARD_RESERVED); delay(10);
      } else
      {
        digitalWrite(LED, LOW);
        BLE.press_key(HID_KEYBOARD_RIGHT_ARROW); delay(100); // short press sends a right cursor code
        BLE.press_key(HID_KEYBOARD_RESERVED); delay(10);
      }
    } else if (rightBP == LOW and rightBPPrev == HIGH) // button is pressed - start counting time
    {
      startRight = millis();
      digitalWrite(LED, HIGH);
    }

	// manage left button state
    if (leftBP == HIGH and leftBPPrev == LOW) // left button is released
    {
      endLeft = millis();
      if (endLeft - startLeft > 4000) // very long press >4s - try to set HID mode
      {
        Serial.print("AT+FSM=FSM_HID_USB_COM_BLE_AT") + Serial.println(); delay(10);
        Serial.print("AT+ROLE=ROLE_PERIPHERAL") + Serial.println(); delay(10);

        blinkLED(250, 4);
      } else if (endLeft - startLeft > 2000) //  long press >2s & <4s - set UART transparent mode to program the Arduino processor
      {
        Serial.print("AT+FSM=FSM_TRANS_USB_COM_BLE") + Serial.println(); delay(10);
        Serial.print("AT+ROLE=ROLE_CENTRAL") + Serial.println(); delay(10);
        blinkLED(500, 2);
      } else
      {
	  // short press send a left cursor code
        digitalWrite(LED, LOW);
        BLE.press_key(HID_KEYBOARD_LEFT_ARROW); delay(100);
        BLE.press_key(HID_KEYBOARD_RESERVED); delay(10);

      }

    } else if (leftBP == LOW and leftBPPrev == HIGH) // button is pressed - start measurng time
    {
      digitalWrite(LED, HIGH);
      startLeft = millis();

    }

	// memorize previous state
    leftBPPrev = leftBP; // memorise previous state
    rightBPPrev  = rightBP;
    delay(100);
  }
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

