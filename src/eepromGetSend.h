
void getEEprom() {

// Load Max/Min from EEPROM

        EEPROM.get(0,setTemp);

        EEPROM.get(4,maxTemp1);
        EEPROM.get(8,minTemp1);
        EEPROM.get(12,maxTemp2);
        EEPROM.get(16,minTemp2);
        EEPROM.get(20,maxTemp3);
        EEPROM.get(24,minTemp3);

}



void putEEprom(){

        EEPROM.put(0,setTemp);

        EEPROM.put(4,maxTemp1);
        EEPROM.put(8,minTemp1);
        EEPROM.put(12,maxTemp2);
        EEPROM.put(16,minTemp2);
        EEPROM.put(20,maxTemp3);
        EEPROM.put(24,minTemp3);


        EEPROM.commit();

}

/*
   maxTemp1=0;
   maxTemp2=0;
   minTemp1=25;
   minTemp2=25;

   maxTemp3=0;
   minTemp3=25;
 */





// Load setTemp from EEPROM
//  eeprom=EEPROM.read(4);
//  setTemp=eeprom/10;
