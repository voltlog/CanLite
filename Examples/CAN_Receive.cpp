/* Demo code for the Voltlog CanLite development board 
*  https://voltlog.com/y/canlite
*
*  This code will continously forward packets received on the CAN interface 
*  to the serial port at the configured baud rates.
*
*  Created 8-Apr-2022, based on original example from sandeepmistry Arduino CAN library
*  Uses Arduino CAN library by sandeepmistry https://github.com/sandeepmistry/arduino-CAN
*/
#include <Arduino.h>
#include <CAN.h>

#define LED 5      // On board LED
//#define OUT1 17     // OUT1 via U2
//#define STATUS1 16  // STATUS of U2
//#define OUT2 19     // OUT2 via U3
//#define STATUS2 18  // STATUS of U2
#define CANRX GPIO_NUM_22 // Define CAN RX pin
#define CANTX GPIO_NUM_23 // Define CAN TX pin
#define CANSPEED 500E3  //define can speed

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("Voltlog CanLite CAN Receiver Demo");
  Serial.print("Setting up CAN interface for ");
  Serial.print((int)CANSPEED);
  Serial.print(" baud rate \r\n");

  CAN.setPins(CANRX, CANTX);

  // start the CAN bus at 500 kbps
  if (!CAN.begin(CANSPEED)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }
}

void loop() {
  // try to parse packet
  int packetSize = CAN.parsePacket();

  if (packetSize) {
    // received a packet
    Serial.print("Received ");

    if (CAN.packetExtended()) {
      Serial.print("extended ");
    }

    if (CAN.packetRtr()) {
      // Remote transmission request, packet contains no data
      Serial.print("RTR ");
    }

    Serial.print("packet with id 0x");
    Serial.print(CAN.packetId(), HEX);

    if (CAN.packetRtr()) {
      Serial.print(" and requested length ");
      Serial.println(CAN.packetDlc());
    } else {
      Serial.print(" and length ");
      Serial.print(packetSize);
      Serial.print(" [");

      // only print packet data for non-RTR packets
      while (CAN.available()) {
        Serial.print((char)CAN.read(), HEX);
      }
      Serial.print("] \r\n");
    }

    Serial.println();
  }
}