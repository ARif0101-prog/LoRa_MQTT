
#include <LoRa.h>
#include "boards.h"
int incomingByte = 0;
long lastSendTime = 0;        // last send time
int interval = 2000;          // interval between sends
void setup()
{
    initBoard();
    // When the power is turned on, a delay is required.
    delay(1500);
    Serial.begin(9600);
    Serial.println("LoRa Receiver");

    LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DIO0_PIN);
    if (!LoRa.begin(LoRa_frequency)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }
}

void loop()
{
    if (millis() - lastSendTime > interval) {
    //String message = "HeLoRa World!";   // send a message
    sendMessage("4");
    //Serial.println("Sending " + message);
    lastSendTime = millis();            // timestamp the message
    interval = random(2000) + 1000;    // 2-3 seconds
  }

  // parse for a packet, and call onReceive with the result:
  onReceive(LoRa.parsePacket());
}

void sendMessage(String outgoing) {
  LoRa.beginPacket();                   // start packet
  LoRa.print(outgoing);              // add destination address

  LoRa.endPacket();                     // finish packet and send it
                          // increment message ID
}


//     int packetSize = LoRa.parsePacket();
//     if (packetSize) {
//         // received a packet
//         Serial.print("Received packet '");

//         String recv = "";
//         // read packet
//         while (LoRa.available()) {
//             recv += (char)LoRa.read();
//         }

//         Serial.println(recv);

//         // print RSSI of packet
//         Serial.print("' with RSSI ");
//         Serial.println(LoRa.packetRssi());

//     }
void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return
if (packetSize){
  // read packet header bytes:
 // int recipient = LoRa.read();          // recipient address
 // byte sender = LoRa.read();            // sender address
  //byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length

  String incoming = "";

  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  // if (incomingLength != incoming.length()) {   // check length for error
  //   Serial.println("error: message length does not match length");
  //   return;                             // skip rest of function
  // }
  Serial.println(incoming);
          Serial.print("' with RSSI ");
        Serial.println(LoRa.packetRssi());
  // if the recipient isn't this device or broadcast,
  // if (recipient != localAddress && recipient != 0xFF) {
  //   Serial.println("This message is not for me.");
  //   return;                             // skip rest of function
  // }

  // if message is for this device, or broadcast, print details:
  // Serial.println("Received from: 0x" + String(sender, HEX));
  // Serial.println("Sent to: 0x" + String(recipient, HEX));
  // Serial.println("Message ID: " + String(incomingMsgId));
  // Serial.println("Message length: " + String(incomingLength));
  // Serial.println("Message: " + incoming);
  // Serial.println("RSSI: " + String(LoRa.packetRssi()));
  // Serial.println("Snr: " + String(LoRa.packetSnr()));
  // Serial.println();

}

}



  // while (Serial.available() > 0) {
  //   incomingByte = Serial.read();
    // LoRa.beginPacket();
    // LoRa.print(4);
    // LoRa.endPacket();

  // }
    // try to parse packet
//     int packetSize = LoRa.parsePacket();
//     if (packetSize) {
//         // received a packet
//         Serial.print("Received packet '");

//         String recv = "";
//         // read packet
//         while (LoRa.available()) {
//             recv += (char)LoRa.read();
//         }

//         Serial.println(recv);

//         // print RSSI of packet
//         Serial.print("' with RSSI ");
//         Serial.println(LoRa.packetRssi());

//     }
//     //Serial.flush();
// }

double DistanceBetween2P(double lat1, double long1, double lat2, double long2)
{
  // returns distance in meters between two positions, both specified
  // as signed decimal-degrees latitude and longitude. Uses great-circle
  // distance computation for hypothetical sphere of radius 6372795 meters.
  // Because Earth is no exact sphere, rounding errors may be up to 0.5%.
  // Courtesy of Maarten Lamers
  double delta = radians(long1-long2);
  double sdlong = sin(delta);
  double cdlong = cos(delta);
  lat1 = radians(lat1);
  lat2 = radians(lat2);
  double slat1 = sin(lat1);
  double clat1 = cos(lat1);
  double slat2 = sin(lat2);
  double clat2 = cos(lat2);
  delta = (clat1 * slat2) - (slat1 * clat2 * cdlong);
  delta = sq(delta);
  delta += sq(clat2 * sdlong);
  delta = sqrt(delta);
  double denom = (slat1 * slat2) + (clat1 * clat2 * cdlong);
  delta = atan2(delta, denom);
  return delta * 6372795;
}

double StringToDouble(String & str)
{
  return atof(str.c_str());
}
