#include "Arduino.h"
#include <DFBLE.h>

#define leftBPpin 2
#define rightBPpin 3
#define LED 13
#define DEBUG true

void setup() {
  // initialize Serial:
  Serial.begin(112500);

  pinMode(leftBPpin, INPUT_PULLUP);
  pinMode(rightBPpin, INPUT_PULLUP);
  pinMode(LED, OUTPUT);

  delay(200);
}
void loop() {
  // put your main code here, to run repeatedly:
  int leftBP, leftBPPrev;
  int rightBP, rightBPPrev;
  long startLeft, endLeft; // time in ms
  long startRight, endRight; 
  leftBPPrev = HIGH;
  rightBPPrev = HIGH;


  while (1)
  {
    leftBP = digitalRead(leftBPpin);
    rightBP = digitalRead(rightBPpin);

    if (rightBP == HIGH and rightBPPrev == LOW) // right button released
    {
      endRight = millis();
      if (endRight - startRight > 500) // long press
      {
        BLE.press_key(HID_KEYBOARD_C); delay(100);
        BLE.press_key(HID_KEYBOARD_RESERVED); delay(10);
      } else
      {
        digitalWrite(LED, LOW);
        BLE.press_key(HID_KEYBOARD_RIGHT_ARROW); delay(100);
        BLE.press_key(HID_KEYBOARD_RESERVED); delay(10);
      }
    } else if (rightBP == LOW and rightBPPrev == HIGH) // button is pressed
    {
      startRight = millis();
      digitalWrite(LED, HIGH);
    }


    if (leftBP == HIGH and leftBPPrev == LOW) // left button is released
    {
      endLeft = millis();
      if (endLeft - startLeft > 4000) // very long press >4s
      {
        Serial.print("AT+FSM=FSM_HID_USB_COM_BLE_AT") + Serial.println(); delay(10);
        Serial.print("AT+ROLE=ROLE_PERIPHERAL") + Serial.println(); delay(10);

        blinkLED(250, 4);
      } else if (endLeft - startLeft > 2000) //  long press >2s & <4s
      {
        Serial.print("AT+FSM=FSM_TRANS_USB_COM_BLE") + Serial.println(); delay(10);
        Serial.print("AT+ROLE=ROLE_CENTRAL") + Serial.println(); delay(10);
        blinkLED(500, 2);
      } else
      {
        digitalWrite(LED, LOW);
        BLE.press_key(HID_KEYBOARD_LEFT_ARROW); delay(100);
        BLE.press_key(HID_KEYBOARD_RESERVED); delay(10);

      }

    } else if (leftBP == LOW and leftBPPrev == HIGH) // button is pressed
    {
      digitalWrite(LED, HIGH);
      startLeft = millis();

    }

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

