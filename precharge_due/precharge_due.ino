#include "DueCANLayer.h"

// CAN Layer functions
extern byte canInit(byte cPort, long lBaudRate);
extern byte canTx(byte cPort, long lMsgID, bool bExtendedFormat, byte* cData, byte cDataLen);
extern byte canRx(byte cPort, long* lMsgID, bool* bExtendedFormat, byte* cData, byte* cDataLen);

const int PRECHARGE_RELAY_PIN = 2;
const int SOURCE_RELAY_PIN = 3;
const int CAN_THRESHOLD = 3 * 1000;

void setup()
{
  // Set the serial interface baud rate
  Serial.begin(115200);

  pinMode(PRECHARGE_RELAY_PIN, OUTPUT);
  digitalWrite(PRECHARGE_RELAY_PIN, HIGH);

  pinMode(SOURCE_RELAY_PIN, OUTPUT);
  digitalWrite(SOURCE_RELAY_PIN, HIGH);
  
  // Initialize both CAN controllers
  if(canInit(0, CAN_BPS_250K) == CAN_OK)
    Serial.print("CAN0: Initialized Successfully.\n\r");
  else
    Serial.print("CAN0: Initialization Failed.\n\r");
  
}// end setup

void loop()
{  
  int timer = 0;
  int lastCanFrame = 0;

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
        lastCanFrame = timer;
        byte relayState = cRxData[0];

        if (relayState & 1) 
        {
          if (timer >= 6000)
          {
            digitalWrite(SOURCE_RELAY_PIN, LOW);
            Serial.print("Set source low\n");
            digitalWrite(PRECHARGE_RELAY_PIN, LOW);
            Serial.print("Set precharge low\n");
            if (canTx(0, 0x6B4, false, cTxOff, 1) != CAN_OK) {
              Serial.print("Error sending message\n");
            }
          }
          else if (timer >= 5000)
          {
            digitalWrite(SOURCE_RELAY_PIN, LOW);
            Serial.print("Set source low\n");
            digitalWrite(PRECHARGE_RELAY_PIN, LOW);
            Serial.print("Set precharge low\n");
            if (canTx(0, 0x6B4, false, cTxOn, 1) != CAN_OK) {
              Serial.print("Error sending message\n");
            }
          }
          else
          {
            digitalWrite(SOURCE_RELAY_PIN, HIGH);
            Serial.print("Set source high\n");
            digitalWrite(PRECHARGE_RELAY_PIN, HIGH);
            Serial.print("Set precharge high\n");
            if (canTx(0, 0x6B4, false, cTxOn, 1) != CAN_OK) {
              Serial.print("Error sending message\n");
            }
          }
          Serial.print("Discharge enabled");
          Serial.print("\n"); 
        }
        else
        {
          Serial.print("Set source high\n");
          digitalWrite(SOURCE_RELAY_PIN, HIGH);
          Serial.print("Set digital high\n");
          digitalWrite(PRECHARGE_RELAY_PIN, HIGH);
        }
      }
    } // end if

    if (timer - lastCanFrame > CAN_THRESHOLD) 
    {
      Serial.print("Set source high\n");
      digitalWrite(SOURCE_RELAY_PIN, HIGH);
      Serial.print("Set digital high\n");
      digitalWrite(PRECHARGE_RELAY_PIN, HIGH);
    }

  }// end while

}// end loop
