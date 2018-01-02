// Heater control logic

void heatControl() {
        Serial.println("heatControl");
        if (DEBUG) Telnet.println("heatControl");

        if (fanHeat > 0) {

                digitalWrite(16,HIGH);
                //PORTB = (1<<PB1);


                Blynk.virtualWrite(V0,1023);

                digitalWrite(14,HIGH);
                //PORTB = (1<<PB2);

                Blynk.virtualWrite(V28,1023);
                Blynk.virtualWrite(V29,1023);

                //check to see if FreeHeat fan is still required, of not turn it off
                if (probe2 < probe1) digitalWrite (12,LOW);
                if (probe2 < probe1) Blynk.virtualWrite(30,0);

                //End if
        }

        else
        {
                //Turn Fan Heat off
                digitalWrite(14,LOW);
                //PORTB = (0<<PB2);
                Blynk.virtualWrite(V28,0);
                Blynk.virtualWrite(V29,0);


                // Check temp in shed vs temp in Workshop, if shed is warmer turn on FreeHeat fan
                if (probe2 > probe1) digitalWrite (12,HIGH);
                if (probe2 > probe1) Blynk.virtualWrite(30,1023);

                //Turn off FreeHeat fan when shed is no longer warmer than the workshop
                if (probe2 < probe1) digitalWrite (12,LOW);
                if (probe2 < probe1) Blynk.virtualWrite(30,0);


                // Check temp inside workshop and compare to set point and turn on Heater if required
                if (probe1 < (setTemp - HYSTERESIS)) digitalWrite(16, HIGH);
                if (probe1 > (setTemp + HYSTERESIS)) digitalWrite(16, LOW);

                //if (probe1 < (setTemp - HYSTERESIS)) PORTB = (1<<PB1);;
                //if (probe1 > (setTemp + HYSTERESIS)) PORTB = (0<<PB1);;


                // Switch virtual Blynk app LED on/off to match heater
                if (probe1 < (setTemp - HYSTERESIS)) Blynk.virtualWrite(V0, 1023);
                if (probe1 > (setTemp + HYSTERESIS)) Blynk.virtualWrite(V0, 0);

                //End else
        }
//End heatControl
}
