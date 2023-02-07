#include <EEPROM.h>
#include <Wire.h>
#include <hd44780.h>                       // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header
#include "MCP3208.h"
#include "MilliTimer.h"
#include "SerialChecker.h"
#include "DisplayHelper.h"

//////////// CALIBRATION AND INPUT SETTINGS
const uint8_t coarseCoeffA_Address = 1;
const uint8_t coarseCoeffB_Address = 1 + 5;
const uint8_t fineCoeffA_Address = 1 + 10;
const uint8_t fineCoeffB_Address = 1 + 15;
const uint8_t PD_CoeffA_Address = 1 + 20;
const uint8_t PD_CoeffB_Address = 1 + 25;
const uint8_t timeStepAddress = 1 + 30;

float coarseCoeffA = -12.256;
float coarseCoeffB = 0.195/9.788;
float fineCoeffA = 40.14;
float fineCoeffB = 0.0024395;
float PD_CoeffA = 0.0;
float PD_CoeffB = 1.0;
uint16_t timeStep = 100; // milliseconds

MCP3208 adc(5); // pin 5 used as chip select on an MCP3204
const uint8_t coarseTempChan = 1;
const uint8_t fineTempChan = 2;
const uint8_t PD_Chan = 3;

float floatCoarseTemp = 0.0;
float floatFineTemp = 0.0;
float floatPD = 0.0;
bool readingAvailable = false;

MilliTimer dataTimer(timeStep); // interval between data points in milliseconds

//////////// DISPLAY SETTINGS
hd44780_I2Cexp lcd; // declare lcd object: auto locate & config exapander chip
const uint8_t LCDcolumns = 20;
const uint8_t LCDrows = 4;
/////////// DISPLAY BUFFERS TO SPEED SCREEN UPDATES
DisplayHelper coarseT_Buffer(6);
DisplayHelper fineT_Buffer(6);
DisplayHelper PD_Buffer(6);
DisplayHelper timeStepBuffer(5);

//////////// SERIAL SETTINGS
SerialChecker sc; // default baud is 250000 bps
bool serialReadingsEnabled = false;

void setup(){
    EEPROM.begin(36); // create a 32byte EEPROM emulation
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
    if(EEPROM.read(coarseCoeffA_Address - 1) == 'A'){
        EEPROM.get(coarseCoeffA_Address, coarseCoeffA);
    }
    if(EEPROM.read(coarseCoeffB_Address - 1) == 'B'){
        EEPROM.get(coarseCoeffB_Address, coarseCoeffB);
    }
    if(EEPROM.read(fineCoeffA_Address - 1) == 'A'){
        EEPROM.get(fineCoeffA_Address, fineCoeffA);
    }
    if(EEPROM.read(fineCoeffB_Address - 1) == 'B'){
        EEPROM.get(fineCoeffB_Address, fineCoeffB);
    }
    if(EEPROM.read(PD_CoeffA_Address - 1) == 'A'){
        EEPROM.get(PD_CoeffA_Address, PD_CoeffA);
    }
    if(EEPROM.read(PD_CoeffB_Address - 1) == 'B'){
        EEPROM.get(PD_CoeffB_Address, PD_CoeffB);
    }
    if(EEPROM.read(timeStepAddress - 1) == 'T'){
        EEPROM.get(timeStepAddress, timeStep);
    }
}

void initDisplay(){
    lcd.begin(LCDcolumns, LCDrows);
    lcd.noLineWrap();
    lcd.setCursor(0, 0);
    lcd.print("Coarse Temp:       C");
    lcd.setCursor(0, 1);
    lcd.print("  Fine Temp:       C");
    lcd.setCursor(0, 2);
    lcd.print(" Photodiode:       V");
    lcd.setCursor(0, 3);
    lcd.print("  Time Step:      ms");
    lcd.setCursor(13, 3);
    updateTimeValue(13, 3, timeStep);
}

void updateTimeValue(uint8_t column, uint8_t row, uint16_t value){
    uint8_t n = timeStepBuffer.update(value);
    for(uint8_t i = 0; i < n; i++){
        lcd.setCursor(column + timeStepBuffer.popIndex(), row);
        lcd.print(timeStepBuffer.popChar());
    }
}

void updateCoarseTemp(uint8_t column, uint8_t row, float value, uint8_t n_dp){
    uint8_t n = coarseT_Buffer.update(value, n_dp);
    for(uint8_t i = 0; i < n; i++){
        lcd.setCursor(column + coarseT_Buffer.popIndex(), row);
        lcd.print(coarseT_Buffer.popChar());
    }
}

void updateFineTemp(uint8_t column, uint8_t row, float value, uint8_t n_dp){
    uint8_t n;
    if(floatFineTemp < fineCoeffA || floatFineTemp > (fineCoeffA + fineCoeffB * 4095.0)){
        n = fineT_Buffer.update("-----");
        // lcd.print("-----");
    }
    else{
        n = fineT_Buffer.update(value, n_dp);
        // lcd.print(floatFineTemp, 2);
    }
    for(uint8_t i = 0; i < n; i++){
        lcd.setCursor(column + fineT_Buffer.popIndex(), row);
        lcd.print(fineT_Buffer.popChar());
    }
}

void updatePD(uint8_t column, uint8_t row, float value, uint8_t n_dp){
    uint8_t n = PD_Buffer.update(value, n_dp);
    for(uint8_t i = 0; i < n; i++){
        lcd.setCursor(column + PD_Buffer.popIndex(), row);
        lcd.print(PD_Buffer.popChar());
    }
}

void readAnalogInputs(){
    static int32_t avCoarseTemp = 0;
    static int32_t avFineTemp = 0;
    static int32_t avPD = 0;
    static uint32_t nSamples = 0;
    if(!dataTimer.timedOut(true) || nSamples == 0){ //prevent divide by zero is timestep is too small for a reading to have taken place
        avCoarseTemp += adc.read(coarseTempChan);
        avFineTemp += adc.read(fineTempChan);
        avPD += adc.read(PD_Chan);
        nSamples++;
    }
    else{
        avCoarseTemp /= nSamples;
        avFineTemp /= nSamples;
        avPD /= nSamples;

        floatCoarseTemp = coarseCoeffA + coarseCoeffB * (5000.0/4095.0*float(avCoarseTemp));
        floatFineTemp = fineCoeffA + fineCoeffB * float(avFineTemp);
        floatPD = PD_CoeffA + PD_CoeffB * 5.0/4095.0 * float(avPD);

        avCoarseTemp = 0;
        avFineTemp = 0;
        avPD = 0;
        Serial.print("N samp: ");
        Serial.println(nSamples);
        nSamples = 0;
        readingAvailable = true;
    }
}

void sendSerialUpdate(){
    Serial.print(floatCoarseTemp, 1);
    Serial.print(", ");
    if(floatFineTemp < fineCoeffA || floatFineTemp > (fineCoeffA + fineCoeffB * 4095.0)){
        Serial.print("-");
    }
    else{
        Serial.print(floatFineTemp, 2);
    }
    Serial.print(", ");
    Serial.println(floatPD, 3);
}

void updateDisplay(){
    // lcd.setCursor(13, 0);
    // lcd.print("      ");
    // lcd.setCursor(13, 0);
    // lcd.print(floatCoarseTemp, 1);
    updateCoarseTemp(13, 0, floatCoarseTemp, 1);
    // lcd.setCursor(13, 1);
    // lcd.print("      ");
    // lcd.setCursor(13, 1);
    // if(floatFineTemp < fineCoeffA || floatFineTemp > (fineCoeffA + fineCoeffB * 4095.0)){
    //     lcd.print("-----");
    // }
    // else{
    //     lcd.print(floatFineTemp, 2);
    // }
    updateFineTemp(13, 1, floatFineTemp, 2);
    // lcd.setCursor(13, 2);
    // lcd.print("      ");
    // lcd.setCursor(13, 2);
    // lcd.print(floatPD, 3);
    updatePD(13, 2, floatPD, 3);
}

void checkSerial(){
    if(sc.check()){
        if(sc.contains("SCA")){
            coarseCoeffA = sc.toFloat(3);
        }
        else if(sc.contains("SCB")){
            coarseCoeffB = sc.toFloat(3);
        }
        else if(sc.contains("SFA")){
            fineCoeffA = sc.toFloat(3);
        }
        else if(sc.contains("SFB")){
            fineCoeffB = sc.toFloat(3);
        }
        else if(sc.contains("SPA")){
            PD_CoeffA = sc.toFloat(3);
        }
        else if(sc.contains("SPB")){
            PD_CoeffB = sc.toFloat(3);
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
        else if(sc.contains("SE")){
            // Save all to EEPROM
            EEPROM.put(coarseCoeffA_Address - 1, 'A'); // Write CAL so the main code can check for this pattern. If it isn't present, it will not update OSCCAL.
            EEPROM.put(coarseCoeffB_Address - 1, 'B');
            EEPROM.put(fineCoeffA_Address - 1, 'A');
            EEPROM.put(fineCoeffB_Address - 1, 'B');
            EEPROM.put(PD_CoeffA_Address - 1, 'A');
            EEPROM.put(PD_CoeffB_Address - 1, 'B');
            EEPROM.put(timeStepAddress - 1, 'T');
            EEPROM.put(coarseCoeffA_Address, coarseCoeffA);
            EEPROM.put(coarseCoeffB_Address, coarseCoeffB);
            EEPROM.put(fineCoeffA_Address, fineCoeffA);
            EEPROM.put(fineCoeffB_Address, fineCoeffB);
            EEPROM.put(PD_CoeffA_Address, PD_CoeffA);
            EEPROM.put(PD_CoeffB_Address, PD_CoeffB);
            EEPROM.put(timeStepAddress, timeStep);
            EEPROM.commit(); // must do this after any writes on ESP32
        }
        else if(sc.contains("ID")){
            Serial.println("CO"); // for thermal diffusivity
        }
        else if(sc.contains("GT")){
            // Get time step
            Serial.println(timeStep);
        }
        else if(sc.contains("GCA")){
            Serial.println(coarseCoeffA, 6);
        }
        else if(sc.contains("GCB")){
            Serial.println(coarseCoeffB, 6);
        }
        else if(sc.contains("GFA")){
            Serial.println(fineCoeffA, 6);
        }
        else if(sc.contains("GFB")){
            Serial.println(fineCoeffB, 6);
        }
        else if(sc.contains("GPA")){
            Serial.println(PD_CoeffA, 6);
        }
        else if(sc.contains("GPB")){
            Serial.println(PD_CoeffB, 6);
        }
        else if(sc.contains("ER")){
            serialReadingsEnabled = true;
        }
        else if(sc.contains("DR")){
            serialReadingsEnabled = false;
        }
    }
}