#include <EEPROM.h>
#include <Wire.h>
#include <hd44780.h>                       // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header
#include "MCP3208.h"
#include "MilliTimer.h"
#include "SerialChecker.h"
#include "DisplayHelper.h"

//////////// CALIBRATION AND INPUT SETTINGS
int analogOffsetAddress = 1; // For the EEPROM
int16_t analogOffset = 0;
int timeStepAddress = 4; // For the EEPROM
uint16_t timeStep = 100; // milliseconds
MCP3208 adc(5); // pin 5 used as chip select on an MCP3204
const uint8_t tempChan = 1;
float floatTemp = 0.0;
bool readingAvailable = false;
MilliTimer dataTimer(timeStep); // interval between data points in milliseconds

//////////// DISPLAY SETTINGS
hd44780_I2Cexp lcd; // declare lcd object: auto locate & config exapander chip
const uint8_t LCDcolumns = 20;
const uint8_t LCDrows = 4;
/////////// DISPLAY BUFFERS TO SPEED SCREEN UPDATES
DisplayHelper T_Buffer(6);
DisplayHelper timeStepBuffer(5);

//////////// SERIAL SETTINGS
SerialChecker sc; // default baud is 250000 bps
bool serialReadingsEnabled = false;

void setup(){
    EEPROM.begin(32); // create a 32byte EEPROM emulation
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
    if(EEPROM.read(analogOffsetAddress - 1) == 'A'){
        EEPROM.get(analogOffsetAddress, analogOffset);
    }
    if(EEPROM.read(timeStepAddress - 1) == 'T'){
        EEPROM.get(timeStepAddress, timeStep);
    }
}

void initDisplay(){
    lcd.begin(LCDcolumns, LCDrows);
    lcd.noLineWrap();
    lcd.setCursor(0, 1);
    lcd.print("Temperature:       C");
    lcd.setCursor(0, 3);
    lcd.print("Time Step:        ms");
    updateTimeValue(13, 3, timeStep);
}

void readAnalogInputs(){
    static int32_t avTemp = 0;
    static uint16_t nSamples = 0;
    if(!dataTimer.timedOut(true) || nSamples == 0){ //prevent divide by zero is timestep is too small for a reading to have taken place
        avTemp += adc.read(tempChan);
        nSamples++;
    }
    else{
        avTemp /= nSamples;
        avTemp -= analogOffset;
        floatTemp = 400.0 * float(avTemp) / (4095.0 - analogOffset);
        avTemp = 0;
        nSamples = 0;
        readingAvailable = true;
    }
}

void sendSerialUpdate(){
    Serial.println(floatTemp, 1);
}

void updateDisplay(){
    // lcd.setCursor(13, 1);
    // lcd.print("      ");
    // lcd.setCursor(13, 1);
    // lcd.print(floatTemp, 1);
    updateTemp(13, 1, floatTemp, 2);
}

void updateTimeValue(uint8_t column, uint8_t row, uint16_t value){
    uint8_t n = timeStepBuffer.update(value);
    for(uint8_t i = 0; i < n; i++){
        lcd.setCursor(column + timeStepBuffer.popIndex(), row);
        lcd.print(timeStepBuffer.popChar());
    }
}

void updateTemp(uint8_t column, uint8_t row, float value, uint8_t n_dp){
    uint8_t n = T_Buffer.update(value, n_dp);
    for(uint8_t i = 0; i < n; i++){
        lcd.setCursor(column + T_Buffer.popIndex(), row);
        lcd.print(T_Buffer.popChar());
    }
}

void checkSerial(){
    if(sc.check()){
        if(sc.contains("SA")){
            analogOffset = sc.toInt16();
        }
        else if(sc.contains("ST")){
            timeStep = sc.toInt16();
            dataTimer.updateTimeOut(timeStep);
            updateTimeValue(13, 3, timeStep);
        }
        else if(sc.contains("SE")){
            // Save all to EEPROM
            EEPROM.put(analogOffsetAddress - 1, 'A'); // Write CAL so the main code can check for this pattern. If it isn't present, it will not update OSCCAL.
            EEPROM.put(analogOffsetAddress, analogOffset);
            EEPROM.put(timeStepAddress - 1, 'T');
            EEPROM.put(timeStepAddress, timeStep);
            EEPROM.commit(); // must do this after any writes on ESP32
        }
        else if(sc.contains("ID")){
            Serial.println("AL"); // for alloy
        }
        else if(sc.contains("GT")){
            // Get time step
            Serial.println(timeStep);
        }
        else if(sc.contains("GA")){
            Serial.println(analogOffset);
        }
        else if(sc.contains("ER")){
            serialReadingsEnabled = true;
        }
        else if(sc.contains("DR")){
            serialReadingsEnabled = false;
        }
    }
}