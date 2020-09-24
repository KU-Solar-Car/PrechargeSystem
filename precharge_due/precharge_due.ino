#include "DueCANLayer.h"

// CAN Layer functions
extern byte canInit(byte cPort, long lBaudRate);
extern byte canTx(byte cPort, long lMsgID, bool bExtendedFormat, byte* cData, byte cDataLen);
extern byte canRx(byte cPort, long* lMsgID, bool* bExtendedFormat, byte* cData, byte* cDataLen);

const int RELAY_PIN = 2;

void setup()
{
  // Set the serial interface baud rate
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  
  // Initialize both CAN controllers
  if(canInit(0, CAN_BPS_250K) == CAN_OK)
    Serial.print("CAN0: Initialized Successfully.\n\r");
  else
    Serial.print("CAN0: Initialization Failed.\n\r");
  
}// end setup

void loop()
{  
  int timer = 0;
  while(1)  // Endless loop
  {
    delay(1);
    timer++;

    // Check for received message
    long lMsgID;
    bool bExtendedFormat;
    byte cRxData[8];
    byte cDataLen;
    byte cTxOn[] = {0x1};
    byte cTxOff[] = {0x0};
    if(canRx(0, &lMsgID, &bExtendedFormat, &cRxData[0], &cDataLen) == CAN_OK)
    {
      if (lMsgID == 0x6B5) 
      {
        byte relayState = cRxData[0];

        if (relayState & 1) 
        {
          if (timer >= 5000)
          {
            digitalWrite(RELAY_PIN, HIGH);
            Serial.print("Set digitial high");
          }
          else
          {
            digitalWrite(RELAY_PIN, LOW);
            Serial.print("Set digitial low");
          }
          if (timer < 6000)
          {
            if (canTx(0, 0x6B4, false, cTxOn, 1) != CAN_OK) {
              Serial.print("Error sending message");
            }
          }
          else
          {
            if (canTx(0, 0x6B4, false, cTxOff, 1) != CAN_OK) {
              Serial.print("Error sending message");
            }
          }
          Serial.print("Discharge enabled");
          Serial.print("\n\r"); 
        }
        else
        {
          Serial.print("Set digital low");
          digitalWrite(RELAY_PIN, LOW);
        }
      }
    }
    else
    {
      Serial.print("Set digital low");
      digitalWrite(RELAY_PIN, LOW);
    } // end if

  }// end while

}// end loop