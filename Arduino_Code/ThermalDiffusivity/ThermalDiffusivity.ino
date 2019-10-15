#include <EEPROM.h>
#include <Wire.h>
#include <hd44780.h>                       // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header
#include "MCP3208.h"
#include "MilliTimer.h"
#include "SerialChecker.h"
#include "DisplayHelper.h"


//////////// CALIBRATION AND INPUT SETTINGS
const uint8_t sizeEEPROM = 32;
const uint8_t T1_CoeffA_Address = 1;
const uint8_t T1_CoeffB_Address = 1 + 5;
const uint8_t T2_CoeffA_Address = 1 + 10;
const uint8_t T2_CoeffB_Address = 1 + 15;
const uint8_t timeStepAddress = 1 + 20;

// T = B * mV + A
// T = 5000/4095*B*DAC + A/B
// 
// T1 is reading 29.45 at 30(set) and (measured), 
// T2 is reading 28.85 at 30(set) and (measured), 9.58 at 
const float scaleF = 5000.0 / 4095.0;
// float T2_CoeffA = -12.56884; // unit 1 numbers
// float T2_CoeffB =0.027075; // unit 1 numbers
// float T1_CoeffA = -13.36566; // unit 1 numbers
// float T1_CoeffB = 0.0273943; // unit 1 numbers
// float T2_CoeffA = -12.5552; // unit 2 numbers
// float T2_CoeffB = 0.027102; // unit 2 numbers
// float T1_CoeffA = -12.9693; // unit 2 numbers
// float T1_CoeffB = 0.027266; // unit 2 numbers
// float T2_CoeffA = -12.49668246; // unit 3 numbers
// float T2_CoeffB =0.02708192282; // unit 3 numbers
// float T1_CoeffA = -12.70033944; // unit 3 numbers
// float T1_CoeffB = 0.027155465; // unit 3 numbers
float T2_CoeffA = -12.72221468; // unit 4 numbers
float T2_CoeffB =0.02714072466; // unit 4 numbers
float T1_CoeffA = -12.4770667; // unit 4 numbers
float T1_CoeffB = 0.02705993776; // unit 4 numbers
uint16_t timeStep = 500; // milliseconds

MCP3208 adc(5); // pin 5 used as chip select on an MCP3204
const uint8_t T1_Chan = 0;
const uint8_t T2_Chan = 1;

float T1_Temp = 0.0;
float T2_Temp = 0.0;
bool readingAvailable = false;

MilliTimer dataTimer(timeStep); // interval between data points in milliseconds

//////////// DISPLAY SETTINGS
hd44780_I2Cexp lcd; // declare lcd object: auto locate & config exapander chip
const uint8_t LCDcolumns = 20;
const uint8_t LCDrows = 4;
/////////// DISPLAY BUFFERS TO SPEED SCREEN UPDATES
DisplayHelper internalT_Buffer(6);
DisplayHelper externalT_Buffer(6);
DisplayHelper timeStepBuffer(5);


//////////// SERIAL SETTINGS
SerialChecker sc; // default baud is 250000 bps
bool serialReadingsEnabled = false;

void setup(){
    EEPROM.begin(sizeEEPROM); // create a 36byte EEPROM emulation
    sc.init(); // setup serial
    sc.enableSTX(true); // use STX default $ symbol and require it at start of messages
    checkEEPROM();
    adc.begin();
    initDisplay();
    dataTimer.reset();
}

void loop(){
    checkSerial();
    readAnalogInputs();
    if(readingAvailable){
        if(serialReadingsEnabled){
            sendSerialUpdate();
        }
        updateDisplay();
        readingAvailable = false;
    }
}

void checkEEPROM(){
    if(EEPROM.read(T1_CoeffA_Address - 1) == 'A'){
        EEPROM.get(T1_CoeffA_Address, T1_CoeffA);
    }
    if(EEPROM.read(T1_CoeffB_Address - 1) == 'B'){
        EEPROM.get(T1_CoeffB_Address, T1_CoeffB);
    }
    if(EEPROM.read(T2_CoeffA_Address - 1) == 'A'){
        EEPROM.get(T2_CoeffA_Address, T2_CoeffA);
    }
    if(EEPROM.read(T2_CoeffB_Address - 1) == 'B'){
        EEPROM.get(T2_CoeffB_Address, T2_CoeffB);
    }
    if(EEPROM.read(timeStepAddress - 1) == 'T'){
        EEPROM.get(timeStepAddress, timeStep);
    }
}

void initDisplay(){
    lcd.begin(LCDcolumns, LCDrows);
    lcd.noLineWrap();
    lcd.setCursor(0, 0);
    lcd.print("Internal T.:       C");
    lcd.setCursor(0, 1);
    lcd.print("External T.:       C");
    
    lcd.setCursor(0, 3);
    lcd.print("Time Step:        ms");
    // uint8_t row = 3;
    // uint8_t column = 13;
    
    // uint8_t n = timeStepBuffer.update(timeStep);
    // for(uint8_t i = 0; i < n; i++){
    //     lcd.setCursor(column + timeStepBuffer.popIndex(), row);
    //     lcd.print(timeStepBuffer.popChar());
    // }
    updateTimeValue(13, 3, timeStep);
    //   lcd.print(timeStep);
}

void updateTimeValue(uint8_t column, uint8_t row, uint16_t value){
    uint8_t n = timeStepBuffer.update(value);
    for(uint8_t i = 0; i < n; i++){
        lcd.setCursor(column + timeStepBuffer.popIndex(), row);
        lcd.print(timeStepBuffer.popChar());
    }
}

void updateInternalTemp(uint8_t column, uint8_t row, float value, uint8_t n_dp){
    uint8_t n = internalT_Buffer.update(value, n_dp);
    for(uint8_t i = 0; i < n; i++){
        lcd.setCursor(column + internalT_Buffer.popIndex(), row);
        lcd.print(internalT_Buffer.popChar());
    }
}

void updateExternalTemp(uint8_t column, uint8_t row, float value, uint8_t n_dp){
    uint8_t n = externalT_Buffer.update(value, n_dp);
    for(uint8_t i = 0; i < n; i++){
        lcd.setCursor(column + externalT_Buffer.popIndex(), row);
        lcd.print(externalT_Buffer.popChar());
    }
}

void readAnalogInputs(){
    static int32_t avT1 = 0;
    static int32_t avT2 = 0;
    static uint16_t nSamples = 0;
    if(!dataTimer.timedOut(true) || nSamples == 0){ //prevent divide by zero is timestep is too small for a reading to have taken place
        avT1 += adc.read(T1_Chan);
        avT2 += adc.read(T2_Chan);
        nSamples++;
    }
    else{
        avT1 /= nSamples;
        avT2 /= nSamples;

        T1_Temp = T1_CoeffA + T1_CoeffB * float(avT1) * scaleF;
        T2_Temp = T2_CoeffA + T2_CoeffB * float(avT2) * scaleF;
        // Serial.print(avT1);
        // Serial.print(", ");
        // Serial.println(avT2);
        avT1 = 0;
        avT2 = 0;
        nSamples = 0;
        readingAvailable = true;
    }
}

void sendSerialUpdate(){
    Serial.print(T1_Temp, 2);
    Serial.print(", ");
    Serial.println(T2_Temp, 2);
    
}

void updateDisplay(){
    // lcd.setCursor(13, 0);
    // lcd.print("      ");
    // lcd.setCursor(13, 0);
    // lcd.print(T1_Temp, 2);
    // lcd.setCursor(13, 1);
    // lcd.print("      ");
    // lcd.setCursor(13, 1);
    // lcd.print(T2_Temp, 2);
    updateInternalTemp(13, 0, T1_Temp, 2);
    updateExternalTemp(13, 1, T2_Temp, 2);
}

void checkSerial(){
    if(sc.check()){
        if(sc.contains("S1A")){
            T1_CoeffA = sc.toFloat(3);
        }
        else if(sc.contains("S1B")){
            T1_CoeffB = sc.toFloat(3);
        }
        else if(sc.contains("S2A")){
            T2_CoeffA = sc.toFloat(3);
        }
        else if(sc.contains("S2B")){
            T2_CoeffB = sc.toFloat(3);
        }
        else if(sc.contains("ST")){
            timeStep = sc.toInt16();
            dataTimer.updateTimeOut(timeStep);
            // lcd.setCursor(13, 3);
            // lcd.print("     ");
            // lcd.setCursor(13, 3);
            // lcd.print(timeStep);
            updateTimeValue(13, 3, timeStep);
        }
        else if(sc.contains("WE")){
            // wipe EEPROM
            for(uint8_t i = 0; i < sizeEEPROM; i++){
                EEPROM.write(i, 0);
            }
            EEPROM.commit();
        }
        else if(sc.contains("SE")){
            // Save all to EEPROM
            EEPROM.put(T1_CoeffA_Address - 1, 'A');
            EEPROM.put(T1_CoeffB_Address - 1, 'B');
            EEPROM.put(T2_CoeffA_Address - 1, 'A');
            EEPROM.put(T2_CoeffB_Address - 1, 'B');
            EEPROM.put(timeStepAddress - 1, 'T');
            EEPROM.put(T1_CoeffA_Address, T1_CoeffA);
            EEPROM.put(T1_CoeffB_Address, T1_CoeffB);
            EEPROM.put(T2_CoeffA_Address, T2_CoeffA);
            EEPROM.put(T2_CoeffB_Address, T2_CoeffB);
            EEPROM.put(timeStepAddress, timeStep);
            EEPROM.commit(); // must do this after any writes on ESP32
        }
        else if(sc.contains("ID")){
            Serial.println("TD"); // for thermal diffusivity
        }
        else if(sc.contains("GT")){
            // Get time step
            Serial.println(timeStep);
        }
        else if(sc.contains("G1A")){
            Serial.println(T1_CoeffA, 6);
        }
        else if(sc.contains("G1B")){
            Serial.println(T1_CoeffB, 6);
        }
        else if(sc.contains("G2A")){
            Serial.println(T2_CoeffA, 6);
        }
        else if(sc.contains("G2B")){
            Serial.println(T2_CoeffB, 6);
        }
        else if(sc.contains("ER")){
            serialReadingsEnabled = true;
        }
        else if(sc.contains("DR")){
            serialReadingsEnabled = false;
        }
    }
}