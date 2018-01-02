
float getTemperature(DeviceAddress deviceAddress)
{

//float tempC = 0;
        tempC = sensors.getTempC(deviceAddress);
        return tempC;

}


void tempUpdate()
{

// Update Temperature readings
        sensors.requestTemperatures();

//Update temp probes
        probe1 = getTemperature(Probe01);
        probe2 = getTemperature(Probe02);
        probe3 = getTemperature(Probe03);
        probe4 = getTemperature(Probe04);

// Check Max/Min
        if (probe1 > maxTemp1) {
                maxTemp1 = probe1;
//  EEPROM.write(0,(maxTemp1*10.00));
        }

        if (probe1 < minTemp1) {
                minTemp1 = probe1;
//  EEPROM.write(1,(minTemp1*10.00));
        }

        if (probe2 > maxTemp2) {
                maxTemp2 = probe2;
//  EEPROM.write(2,(maxTemp2*10.00));
        }

        if (probe2 < minTemp2) {
                minTemp2 = probe2;
//  EEPROM.write(3,(minTemp2*10.00));
        }



        if (probe3 > maxTemp3) {
                maxTemp3 = probe3;
//  EEPROM.write(5,(maxTemp3*10.00));
        }

        if (probe3 < minTemp3) {
                minTemp3 = probe3;
//  EEPROM.write(6,(minTemp3*10.00));
        }


}
