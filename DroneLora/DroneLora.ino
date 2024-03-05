#include <LoRa.h>
#include "boards.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <TinyGPS++.h>
//#include "power.h"
TinyGPSPlus gps;
const char *WIFI_SSID = "tezzzt";
const char *WIFI_PASSWORD = "";
const char *MQTT_HOST = "10.1.1.1";
const int MQTT_PORT = 1883;
const char *MQTT_CLIENT_ID = "ESP8266 NodeMCU";
const char *MQTT_USER = "";
const char *MQTT_PASSWORD = "";
const char *con_STAT="Wi-Fi Failed";
const char *mqtt_STAT="Broker Failed";
const char *lr_STAT="Lora OK";

const char *T_Az = "Az";
const char *T_mod30 ="mod30";
const char *T_mod9960 ="mod9960";
const char *T_fmi ="fmi";
const char *rfl = "rfl";

const char *T_ddm = "ddm";
const char *T_mod90 ="mod90";
const char *T_mod150 ="mod150";
const char *T_sdm ="sdm";
//const char *T_rfl = "rfl";
String d="0";
 
String recv="";
WiFiClient espClient;
//PubSubClient mqttClient(client);

PubSubClient client(espClient);
int counter = 0;
void callback(char* topic, byte* payload, unsigned int length)
{
    payload[length] = '\0';
    //int value = String((char*) payload).toInt();

    Serial.println(topic);
    
    Serial.println(String((char*) payload));
    Serial.flush();
    
    LoRa.beginPacket();
    LoRa.print(topic);//+" "+String((char*) payload));
    LoRa.print(" ");
    LoRa.print(String((char*) payload));
    while (Serial1.available() > 0)
        if (gps.encode(Serial1.read()))
          Serial.println(gps.location.lat(), 6);
          Serial.print(gps.location.lng(), 6);
          LoRa.print(" GPS ");
          LoRa.print(gps.location.lat(), 6);
          LoRa.print(" ");
          LoRa.print(gps.location.lng(),6);
          LoRa.print(" ");
          LoRa.print(gps.altitude.meters());
    LoRa.endPacket();
    // #ifdef HAS_DISPLAY
    // if (u8g2) {
    //       u8g2->setFontDirection(0);
    //       u8g2->clearBuffer();
    //       u8g2->setCursor(0, 15);
    //       u8g2->print("Hello World!");
    //     char buf[256];
    //     u8g2->clearBuffer();
    //     u8g2->drawStr(0, 12, "Transmitting: OK!");
    //     snprintf(buf, sizeof(buf), "Sending: %d", counter);
    //     u8g2->drawStr(0, 30, buf);
    //     u8g2->sendBuffer();
    //}

      // String messageTemp;
      // for (int i = 0; i < length; i++) {
      //   messageTemp += (char)payload[i];
      // }
      //   Serial.print("Message: ");
      //   Serial.println(messageTemp);


}
void setup()
{
    
    initBoard();
    // When the power is turned on, a delay is required.
    delay(1500);

    Serial.begin(115200);

    WiFi.begin(WIFI_SSID);//, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected to Wi-Fi");
    con_STAT="Wi-Fi Ok";

    client.setServer(MQTT_HOST, MQTT_PORT);
    client.setCallback(callback);

    Serial.println("LoRa Sender");
    LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DIO0_PIN);
    LoRa.setTxPower(20);
    if (!LoRa.begin(LoRa_frequency)) {
        Serial.println("Starting LoRa failed!");
        lr_STAT="LoRa Failed";
        while (1);
    }
    while (!client.connected()) {
        //if (client.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD )) 
        if (client.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD )) 
        {
            Serial.println("Connected to MQTT broker");
            mqtt_STAT="Broker Ok";
        } else {
            delay(500);
            Serial.print(".");
            mqtt_STAT="Broker Failed";
        }
    }

    u8g2->setFontDirection(0);
    u8g2->clearBuffer();
        u8g2->setFont(u8g2_font_cu12_tr);
    u8g2->setCursor(0, 15);
    u8g2->print(con_STAT);
    u8g2->setCursor(0, 30);
    u8g2->print(lr_STAT);
    u8g2->setCursor(0, 45);
    u8g2->print(mqtt_STAT);    
    u8g2->sendBuffer();
    client.subscribe(T_Az);
    client.subscribe(T_mod30);
    client.subscribe(T_mod9960);
    client.subscribe(T_fmi); 
    client.subscribe(rfl); 
    client.subscribe(T_ddm);
    client.subscribe(T_mod90);
    client.subscribe(T_mod150);
    client.subscribe(T_sdm);    
}

void loop()
{
    if (!client.connected()){
    reconnect();
  }
    //   int packetSize = LoRa.parsePacket();
    // if (packetSize) {
    //     // received a packet
    //     Serial.print("Received packet '");

    //     while (LoRa.available()) {
    //         recv += (char)LoRa.read();
    //        // string d = recv;
    //     }
    //     d = recv;
    // }


      u8g2->setFontDirection(0);
    u8g2->clearBuffer();
        //u8g2->setFont(u8g2_font_cu12_mn);
        u8g2->setFont(u8g2_font_6x13_tf);
    u8g2->setCursor(0, 15);
    u8g2->print(con_STAT);
    u8g2->setCursor(70, 15);
    u8g2->print("D:");
    u8g2->setCursor(88, 15);
    u8g2->print(d);
    u8g2->setCursor(115, 15);
    u8g2->print(" m");      
    u8g2->setCursor(0, 30);
    u8g2->print(lr_STAT);
    u8g2->setCursor(0, 45);
    u8g2->print(mqtt_STAT);  
    if (PMU->isBatteryConnect()) {
      u8g2->setCursor(0, 60);
      u8g2->print("B: ");
      u8g2->setCursor(20, 60);
      u8g2->print(PMU->getBatteryPercent());
      u8g2->setCursor(40, 60);
      u8g2->print("%");
      u8g2->setCursor(70, 60);
      u8g2->print("V: ");
      u8g2->setCursor(85, 60);
      
      u8g2->print((PMU->getBattVoltage())/1000.0);
      u8g2->setCursor(115, 60);
      u8g2->print("V");

    }
    u8g2->sendBuffer();

        
  client.loop();
    // Serial.print("Sending packet: ");
    // Serial.println(counter);

    // // send packet
    // LoRa.beginPacket();
    // LoRa.print("hello ");
    // LoRa.print(counter);
    // LoRa.endPacket();

// #ifdef HAS_DISPLAY
//     if (u8g2) {
//         char buf[256];
//         u8g2->clearBuffer();
//         u8g2->drawStr(0, 12, "Transmitting: OK!");
//         snprintf(buf, sizeof(buf), "Sending: %d", counter);
//         u8g2->drawStr(0, 30, buf);
//         u8g2->sendBuffer();
//     }
// #endif
//     counter++;
//     delay(10);
  delay(200);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //mqttClient.publish("outTopic","hello world");

      // ... and resubscribe
    
    client.subscribe(T_Az);
    client.subscribe(T_mod30);
    client.subscribe(T_mod9960);
    client.subscribe(T_fmi); 
    client.subscribe(rfl); 
    client.subscribe(T_ddm);
    client.subscribe(T_mod90);
    client.subscribe(T_mod150);
    client.subscribe(T_sdm);    
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
