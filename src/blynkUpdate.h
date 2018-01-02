

void blynkLiveUpdate()
{

// Update Blynk server

        Serial.println("blynkLiveUpdate Start");
        if (DEBUG) Telnet.println("blynkLiveUpdate Start");



// Write main temps
        Blynk.virtualWrite(V1, probe1);
        Blynk.virtualWrite(V2, probe2);
        Blynk.virtualWrite(V3, probe3);
        Blynk.virtualWrite(V32, probe4);




//client.setServer(mqtt_server, 1883);
//client.connect("ESP8266Client");
//delay(5000);

//snprintf (msg, 75, "hello world #%ld", temp1);
//client.publish("dev8266/temp1", probe1);
//client.publish("dev8266/temp2", temp2);
//client.publish("dev8266/hum", hum);
//client.publish("dev8266/volts", volts);





}

void blynkMaxMinUpdate()
{

// Update Blynk server

        Serial.println("blynkMaxMinUpdate Start");
        if (DEBUG) Telnet.println("blynkMaxMinUpdate Start");

// Write Max/Min's
        Blynk.virtualWrite(V4, maxTemp1);
        Blynk.virtualWrite(V5, minTemp1);
        Blynk.virtualWrite(V6, maxTemp2);
        Blynk.virtualWrite(V7, minTemp2);
        Blynk.virtualWrite(V8, maxTemp3);
        Blynk.virtualWrite(V9, minTemp3);

        if (DEBUG) {
                Telnet.println(maxTemp1);
                Telnet.println(minTemp1);
                Telnet.println(maxTemp2);
                Telnet.println(minTemp2);
                Telnet.println(maxTemp3);
                Telnet.println(minTemp3);
                Telnet.println(setTemp);
        }
        //epoch = gettime.getTime();


}


// Read temperature from Slider
BLYNK_WRITE(20) {
        timer.restartTimer(displayTimerID);
        u8g2.setPowerSave(0);
        setTemp=(param.asInt());
        Blynk.virtualWrite(V27,setTemp);
        heatControl();
// EEPROM.write(0,setTemp*10.0);


}


//Reset Max / Min
BLYNK_WRITE(21) {

        maxTemp1=probe1;
        minTemp1=probe1;
        maxTemp2=probe2;
        minTemp2=probe2;
        maxTemp3=probe3;
        minTemp3=probe3;
        putEEprom();
        blynkMaxMinUpdate();


}



//Set Temp to 7.5c
BLYNK_WRITE(25) {
        timer.restartTimer(displayTimerID);
        u8g2.setPowerSave(0);
        setTemp=7.5;
        timer.restartTimer(saveMoneyTimerID);
        // Write Target Temp
        Blynk.virtualWrite(V27,setTemp);
        Blynk.virtualWrite(V20,(setTemp));
        heatControl();

}


//Set Temp to 20c
BLYNK_WRITE(26) {
        timer.restartTimer(displayTimerID);
        u8g2.setPowerSave(0);
        setTemp=20.0;
        timer.restartTimer(saveMoneyTimerID);
        // Write Target Temp
        Blynk.virtualWrite(V27,setTemp);
        Blynk.virtualWrite(V20,(setTemp*10));
        heatControl();

}



// Turn on Fan heater
BLYNK_WRITE(29) {
        timer.restartTimer(displayTimerID);
        u8g2.setPowerSave(0);
        fanHeat = param.asInt();
        timer.restartTimer(saveMoneyTimerID);
        heatControl();

}
