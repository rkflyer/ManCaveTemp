
/*void PrintDateTime(DateTime t)
   {
    char datestr[24];
    sprintf(datestr, "%02d-%02d-%04d  %02d:%02d:%02d  ", t.day(), t.month(), t.year(),   t.hour(), t.minute(), t.second());
    Serial.print(datestr);

   }
 */

#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* mqtt_server = "192.168.1.171";


WiFiClient espClient;
PubSubClient client(espClient);


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






char screenMsg(const char *msgTxt){


        u8g2.firstPage();
        do {
                u8g2.setFont(u8g2_font_6x10_tf);
                u8g2.drawStr( 5, 10, msgTxt);

        } while( u8g2.nextPage() );
//delay(500);
}



void displayOff(){

        u8g2.setPowerSave(1);

}
void displayOn(){

        u8g2.setPowerSave(0);

}




void fanHeaterON(){
        timer.restartTimer(displayTimerID);
        u8g2.setPowerSave(0);

        fanHeat=1;
        digitalWrite(16,HIGH);
        digitalWrite(14,HIGH);


}






void fanHeaterOFF(){
        timer.restartTimer(displayTimerID);
        u8g2.setPowerSave(0);

        fanHeat=0;
        digitalWrite(16,LOW);
        digitalWrite(14,LOW);

}





void restart(){
        ESP.restart();

}

void menuTelnet() {
        Telnet.print("\u001B[2J");
        Telnet.print("\u001B[0;0H");
        Telnet.println("\nWelcome to ManCaveESP8266\n");
        Telnet.println("   Menu Options\n");
        Telnet.println("1. Display Free Heap");
        Telnet.println("2. Display Sensor Temperatures");
        Telnet.println("3. Turn Debug output ON/OFF");
        Telnet.println("4. Turn Fan Heater ON/OFF");
        Telnet.println("\n9. Show Menu (At any time)");
        Telnet.print("\nInput: ");

}
void handleTelnet() {
        // look for Client connect trial
        if (TelnetServer.hasClient()) {
                if (!Telnet || !Telnet.connected()) {
                        if (Telnet) {
                                Telnet.stop();
                                Serial.println("Telnet Client Stop");
                        }
                        Telnet = TelnetServer.available();
                        Serial.println("New Telnet client");
                        Telnet.flush(); // clear input buffer, else you get strange characters
                        menuTelnet();

                }
        }

        while(Telnet.available()) { // get data from Client
                TelnetInput = Telnet.read();
                Serial.print(TelnetInput);
                switch (TelnetInput) {
                case '1':
                        Telnet.print("\n\nFree Heap[B]: ");
                        Telnet.println(ESP.getFreeHeap());
                        Telnet.print("\nInput: ");
                        break;
                case '2':
                        Telnet.print("\n\nProbe1 ");
                        Telnet.println(probe1);
                        Telnet.print("Probe2 ");
                        Telnet.println(probe2);
                        Telnet.print("Probe3 ");
                        Telnet.println(probe3);
                        Telnet.print("\nInput: ");
                        break;
                case '3':
                        DEBUG = !DEBUG;
                        if (!DEBUG) menuTelnet();
                        break;
                case '4':
                        fanHeat=!fanHeat;
                        heatControl();
                        if (fanHeat) Telnet.println("Heater now ON");
                        if (!fanHeat) Telnet.println("Heater now OFF");
                        Telnet.print("\nInput: ");
                        break;


                case '9':
                        menuTelnet();
                        break;


                }


        }

}


void saveMoney() {

        Serial.println("saveMoney");

        Blynk.virtualWrite(V29,0);
        fanHeat = 0;
}






void printDigits(int digits)
{
        // utility for digital clock display: prints preceding colon and leading 0
        Serial.print(":");
        if (digits < 10)
                Serial.print('0');
        Serial.print(digits);
}

/*-------- NTP code ----------

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
        IPAddress ntpServerIP; // NTP server's ip address

        while (Udp.parsePacket() > 0) ; // discard any previously received packets
        Serial.println("Transmit NTP Request");
        // get a random server from the pool
        WiFi.hostByName(ntpServerName, ntpServerIP);
        Serial.print(ntpServerName);
        Serial.print(": ");
        Serial.println(ntpServerIP);
        sendNTPpacket(ntpServerIP);
        uint32_t beginWait = millis();
        while (millis() - beginWait < 1500) {
                int size = Udp.parsePacket();
                if (size >= NTP_PACKET_SIZE) {
                        Serial.println("Receive NTP Response");
                        Udp.read(packetBuffer, NTP_PACKET_SIZE); // read packet into the buffer
                        unsigned long secsSince1900;
                        // convert four bytes starting at location 40 to a long integer
                        secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
                        secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
                        secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
                        secsSince1900 |= (unsigned long)packetBuffer[43];
                        return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
                }
        }
        Serial.println("No NTP Response :-(");
        return 0; // return 0 if unable to get the time
}




// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
        // set all bytes in the buffer to 0
        memset(packetBuffer, 0, NTP_PACKET_SIZE);
        // Initialize values needed to form NTP request
        // (see URL above for details on the packets)
        packetBuffer[0] = 0b11100011; // LI, Version, Mode
        packetBuffer[1] = 0; // Stratum, or type of clock
        packetBuffer[2] = 6; // Polling Interval
        packetBuffer[3] = 0xEC; // Peer Clock Precision
        // 8 bytes of zero for Root Delay & Root Dispersion
        packetBuffer[12] = 49;
        packetBuffer[13] = 0x4E;
        packetBuffer[14] = 49;
        packetBuffer[15] = 52;
        // all NTP fields have been given values, now
        // you can send a packet requesting a timestamp:
        Udp.beginPacket(address, 123); //NTP requests are to port 123
        Udp.write(packetBuffer, NTP_PACKET_SIZE);
        Udp.endPacket();
}
*/