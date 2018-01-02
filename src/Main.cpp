/*

   Title = ManCaveTemp8266

   Update History
   Version 1.0 Final    Installed on 23/03/2017

   1.01 Changed IP Address from remote external address to internal Blynk server on local Network










 */

#include <Arduino.h>

// ESP8266 Libraries
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <EEPROM.h>
#include <SPI.h>
#include <Wire.h>
#include <PubSubClient.h>


// Update these with values suitable for your network.

const char* mqtt_server = "192.168.1.171";
WiFiClient espClient;
PubSubClient client(espClient);

WiFiServer TelnetServer(23);
WiFiClient Telnet;


//#define BLYNK_DEBUG           // disabled debugging to save space
//#define BLYNK_PRINT Telnet

#include <BlynkSimpleEsp8266.h>


// Pin definitions for I2C
#define OLED_SDA    4  // pin 14
#define OLED_SDC    5  // pin 12
#define OLED_ADDR   0x3C
#define ONE_WIRE_BUS_PIN 13



// Setup Global variables
//float Vcc = 0;

float probe1;
float probe2;
float probe3;
float probe4;
float maxTemp1 =5;

float maxTemp2 =6;
float maxTemp3 =6;
float minTemp1 =20;
float minTemp2 =20;
float minTemp3 =20;
float tempC;
float setTemp = 7.0;
float HYSTERESIS = 0.1;
int fanHeat = 0;
int saveMoneyTimerID;
int displayTimerID;
int eeprom;
int DEBUG = 0;
char TelnetInput;
char msg[25];
//char msgTxt;




// NETWORK: Static IP details...
IPAddress ip(192, 168, 1, 84);
IPAddress gateway(192, 168, 1, 254);
IPAddress subnet(255, 255, 255, 0);


//const char* ssid     = "RicHAP";
//const char* password = "22816238";
//const char* ssid     = "RicAP";
//const char* password = "";
//const char* ssid     = "HOGBROOK";
//const char* password = "MaxiTurner";
const char* ssid     = "HOGBROOK6";
const char* password = "E933763592";


String timeNow;
String mins;
String sec;
const int sleepTimeS = 10;



char myIpString[24];
 #define led 2


//Simple timer library and instance called 'timer'
#include <SimpleTimer.h>

//Simple timer instance called 'timer'
SimpleTimer timer;



//One wire and Dallas temp libraries for DS18B20 Temp Sensors
#include <OneWire.h>
#include <DallasTemperature.h>

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS_PIN);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);






// Shed Sensors as Installed
DeviceAddress Probe01 = { 0x28, 0x1E, 0x18, 0x38, 0x05, 0x00, 0x00, 0x75 };
DeviceAddress Probe02 = { 0x28, 0x6F, 0x1B, 0x38, 0x05, 0x00, 0x00, 0x94 };
DeviceAddress Probe03 = { 0x28, 0x45, 0x29, 0xF3, 0x05, 0x00, 0x00, 0x47 };
DeviceAddress Probe04 = { 0x28, 0xFF, 0x8F, 0xA8, 0x90, 0x15, 0x04, 0x03 };  //Probe installed on board

#include <U8g2lib.h>

//U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display
U8G2_SH1106_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display
//U8G2_SH1106_128X64_VCOMH0_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);		// same as the NONAME variant, but maximizes setContrast() range



//#include "images.h"
#include "eepromGetSend.h"
#include "heaterControl.h"
#include "blynkUpdate.h"
#include "miscFunctions.h"
#include "displayFrames.h"
#include "tempUpdate.h"


// Blynk App Auth Token
char auth[] = "e5f5f07a0189470c8970a1736ae1f032";





// Deal with incoming MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

//Connect/Reconnect MQTT Server and Subscribe to topics
  void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
      Serial.print("Attempting MQTT connection...");
      // Attempt to connect
      if (client.connect("ESP8266Client")) {
        Serial.println("connected");



        // Once connected, publish an announcement...
        client.publish("outTopic", "MQQT Has started......");


        // ... and subscribe/resubscribe
        client.subscribe("inTopic");


      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        // Wait 5 seconds before retrying
        delay(5000);
      }
    }
  }











// Main Setup Routine
void setup() {


//Setup Display UI

        u8g2.begin();
        screenMsg("Starting Setup");
        screenMsg("Starting Setup");
        screenMsg("Load EEprom data");
        EEPROM.begin(64);
        getEEprom();


        screenMsg("Attaching Buttons");
        attachInterrupt(15, fanHeaterON, FALLING);
        attachInterrupt(0, fanHeaterOFF, FALLING);


        screenMsg("Starting Telnet");
        TelnetServer.begin();
        TelnetServer.setNoDelay(true);


        screenMsg("Setting Outputs");

        pinMode(16,OUTPUT); // Heaters
        pinMode(14,OUTPUT); // Fan Heater
        pinMode(12,OUTPUT); // Fan fron Shed

        digitalWrite(16, HIGH);
        delay(100);
        digitalWrite(16, LOW);
        delay(100);
        digitalWrite(14, HIGH);
        delay(100);
        digitalWrite(14, LOW);
        delay(100);
        digitalWrite(12, HIGH);
        delay(100);
        digitalWrite(12, LOW);


        Serial.print("Starting Display Setup");


        screenMsg("Starting Serial");
        Serial.begin(115200);
        Serial.println("Starting Setup");
        Serial.println();

        Serial.println("Booting");
        screenMsg("Connecting Wifi");

        WiFi.mode(WIFI_STA);
        // Static IP Setup Info Here...
        WiFi.config(ip, gateway, subnet);

        WiFi.begin(ssid, password);

//  while ((WiFi.status() != WL_CONNECTED)) {
//     Serial.println(WiFi.status());
//     delay(500);
//   }
        delay (3000);
        // Start Blynk and login to Wifi

        screenMsg("Connecting Blynk");
        delay(2000);
        displayOff();

//Blynk.begin(auth,"","",IPAddress(185,46,208,6),8442);
        Blynk.begin(auth,"","",IPAddress(192,168,1,171),8442);
//Blynk.begin(auth,ssid,password,IPAddress(192,168,1,171));

        displayOn();




        Blynk.virtualWrite(V0, 0);
        Blynk.virtualWrite(V29,0);

        Serial.println("Connection requested");
        /*while (WiFi.waitForConnectResult() != WL_CONNECTED) {
           Serial.println("Connection Failed! Rebooting...");
           delay(5000);
           ESP.restart();
           }
         */
        screenMsg("Starting OTA update");

        ArduinoOTA.onStart([]() {
                Serial.println("Start");
                digitalWrite(led,HIGH);
                delay(500);
                digitalWrite(led,LOW);
                delay(200);
                digitalWrite(led,HIGH);
                delay(500);
                digitalWrite(led,LOW);



        });
        ArduinoOTA.onEnd([]() {
                Serial.println("\nEnd");
        });
        ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
                Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
        });
        ArduinoOTA.onError([](ota_error_t error) {
                Serial.printf("Error[%u]: ", error);
                if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
                else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
                else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
                else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
                else if (error == OTA_END_ERROR) Serial.println("End Failed");
        });



        ArduinoOTA.begin();
        Serial.println("Ready");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());


        IPAddress myIp = WiFi.localIP();
        sprintf(myIpString, "%d.%d.%d.%d", myIp[0], myIp[1], myIp[2], myIp[3]);
        //Spark.variable("ipAddress", myIpString, STRING);



        screenMsg("Starting Sensors");
// Start one wire sensors
        sensors.begin();

// set the resolution to 12 bit (Can be 9 to 12 bits .. lower is faster)
        sensors.setResolution(Probe01, 12);
        sensors.setResolution(Probe02, 12);
        sensors.setResolution(Probe03, 12);



        screenMsg("Setting Timers");
// Setup Timers for update libraries
        timer.setInterval(4000, tempUpdate);
        timer.setInterval(4100, oledUpdate);
        timer.setInterval(10000, blynkLiveUpdate);
        timer.setInterval(3600000, putEEprom);
        timer.setInterval(16000, blynkMaxMinUpdate);
        timer.setInterval(15000, heatControl);
        timer.setInterval(86400000, restart); //Restart ESP once every 24hrs
        saveMoneyTimerID = timer.setInterval(3600000, saveMoney);
        displayTimerID = timer.setInterval(3600000, displayOff);


        screenMsg("Starting MQTT");
// Setup MQTT

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  client.connect("ESP8266Client");
  // Once connected, publish an announcement...
  client.publish("outTopic", "MQQT Has started......");
// ... and subscribe/resubscribe
//  client.subscribe("inTopic");





// End Setup
        screenMsg("Setup Complete...");
        delay(1000);
}






// Main Loop
void loop() {

        ArduinoOTA.handle();
        // Deal with Telnet connections
        handleTelnet();

        // Run Blynk main routine
        Blynk.run();


        // Run MQQT Loop
        if (!client.connected()) {
                reconnect();
        }     
        client.loop();
 
        // Timer loop
        timer.run();
        //End Main Loop


}
