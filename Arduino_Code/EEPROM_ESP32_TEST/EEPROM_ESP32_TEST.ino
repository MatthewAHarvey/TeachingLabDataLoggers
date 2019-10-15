#include <EEPROM.h>
#include "MilliTimer.h"
#include "SerialChecker.h"

//////////// SERIAL SETTINGS
SerialChecker sc; // default baud is 250000 bps
bool serialReadingsEnabled = false;

void setup(){
    EEPROM.begin(32); // create a 32byte EEPROM emulation
    sc.init(); // setup serial
    Serial.println(sizeof(123.123));
    float test = 123.123;
    Serial.println(sizeof(test));
}

void loop(){
    checkSerial();
}

void checkSerial(){
    if(sc.check()){
        if(sc.contains("RN")){
            for(uint8_t i = 0; i < 21; i += 5){
                char name = '\0';
                EEPROM.get(i, name);
                Serial.print("Address: ");
                Serial.print(i);
                Serial.print("-> ");
                Serial.println(name);
            }
        }
        else if(sc.contains("RV")){
            for(uint8_t i = 1; i < 25; i += 5){
                float value = 0;
                EEPROM.get(i, value);
                Serial.print("Address: ");
                Serial.print(i);
                Serial.print("-> ");
                Serial.println(value);
            }
        }
        else if(sc.contains("SN")){
            EEPROM.put(0, 'A');
            EEPROM.put(5, 'B');
            EEPROM.put(10, 'C');
            EEPROM.put(15, 'D');
            EEPROM.put(20, 'E');
            EEPROM.commit(); // must do this after any writes on ESP32
        }
        else if(sc.contains("SV")){
            float a = 234.3333;
            float b = 111.1111;
            float c = 2.23;
            float d = 99.933;
            float e = 123.321;
            EEPROM.put(1, a);
            EEPROM.put(6, b);
            EEPROM.put(11, c);
            EEPROM.put(16, d);
            EEPROM.put(21, e);
            // EEPROM.put(1, 234.3333);
            // EEPROM.put(6, 111.1111);
            // EEPROM.put(11, 2.23);
            // EEPROM.put(16, 99.933);
            // EEPROM.put(21, 123.321);
            EEPROM.commit(); // must do this after any writes on ESP32   
        }
        else if(sc.contains("WN")){
            // wipe names
            for(uint8_t i = 0; i < 21; i += 5){
                EEPROM.write(i, 0);
            }
            EEPROM.commit();
        }
        else if(sc.contains("WV")){
            // wipe values
            for(uint8_t i = 1; i < 25; i += 5){
                for( uint8_t j = 0; j < 4; j++){
                    EEPROM.write(i + j, 0);
                }
            }
            EEPROM.commit();
        }
    }
}