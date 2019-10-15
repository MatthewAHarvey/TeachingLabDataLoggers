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
uint16_t LCDMinTimeStep = 20; // if time step is below this, don't update the LCD or the number of samples drops below 63. Each LCD update is taking around 15 ms.

MCP3208 adc(5); // pin 5 used as chip select on an MCP3204
const uint8_t powerSensePin = 33;
const uint8_t scanVchan = 1;
const uint8_t electronVchan = 0;
const uint8_t speciesPin = 14;
volatile bool speciesChanged = false;
volatile bool powerStatusChanged = false;
enum Species { Neon, Mercury };
Species s = Mercury;
float floatScanV = 0.0; // stores the scan voltage as a calibrated floating point number here
float floatElectronV = 0.0; // stores the electron current reading as a calibrated floating point number here
bool readingAvailable = false;
MilliTimer dataTimer(timeStep); // interval between data points in milliseconds
MilliTimer LCDTimer(250);
MilliTimer debounceTimer(100);

//////////// DISPLAY SETTINGS
hd44780_I2Cexp lcd; // declare lcd object: auto locate & config exapander chip
const uint8_t LCDcolumns = 20;
const uint8_t LCDrows = 4;
const int maxLen = 8;
const uint8_t displayFieldLen = 7 + 1;
char oldScanVChar[displayFieldLen] = "       ";
char oldCurrentChar[displayFieldLen] = "       ";
DisplayHelper timeStepBuffer(5);

//////////// SERIAL SETTINGS
SerialChecker sc; // default baud is 250000 bps
bool serialReadingsEnabled = false;

void IRAM_ATTR updateSpeciesChangedFlag();
void IRAM_ATTR updatePowerStatusChangedFlag();

void setup(){
    pinMode(powerSensePin, INPUT);
    pinMode(speciesPin, INPUT_PULLUP);
    EEPROM.begin(32); // create a 32byte EEPROM emulation
    sc.init(); // setup serial
    sc.enableSTX(true); // use STX default $ symbol and require it at start of messages
    attachInterrupt(digitalPinToInterrupt(powerSensePin), updatePowerStatusChangedFlag, CHANGE);
    attachInterrupt(digitalPinToInterrupt(speciesPin), updateSpeciesChangedFlag, CHANGE); // If front panel switch changes position, call setSpecies as an ISR interrupt service routine.
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
    if(powerStatusChanged && debounceTimer.timedOut(true)){
        if(digitalRead(powerSensePin)){
            initDisplay();
            updateDisplay();
            // lcd.backlight();
        }
        else{
            // lcd.noBacklight();
        }
        powerStatusChanged = false;
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
    lcd.setCursor(0, 0);
    lcd.print("Species: ");
    setSpecies();
    lcd.setCursor(0, 1);
    lcd.print("Scan V.:           V");
    lcd.setCursor(0, 2);
    lcd.print("Current:            ");
    lcd.setCursor(0, 3);
    lcd.print("Time Step:        ms");
    updateTimeValue(13, 3, timeStep);
    if(digitalRead(powerSensePin)){
            // lcd.backlight();
        }
        else{
            // lcd.noBacklight();
        }
}

void IRAM_ATTR updateSpeciesChangedFlag(){
    speciesChanged = true;
    debounceTimer.reset();
}

void IRAM_ATTR updatePowerStatusChangedFlag(){ // REMEMBER TO STICK IRAM_ATTR in front of interrupt functions to keep them in IRAM otherwise, core crash.
    powerStatusChanged = true;
    debounceTimer.reset(); // This makes sure the state has settled by the time the main function checks whether to turn backlight on or off
}

void setSpecies(){
    speciesChanged = false;
    lcd.setCursor(11, 0);
    if(digitalRead(speciesPin)){
        s = Neon;
        lcd.print("Neon   ");
    }
    else{
        s = Mercury;
        lcd.print("Mercury");
    }
}

void readAnalogInputs(){
    static int32_t avScanV = 0;
    static int32_t avElectronV = 0;
    static uint16_t nSamples = 0;
    if(!dataTimer.timedOut(true) || nSamples == 0){ //prevent divide by zero is timestep is too small for a reading to have taken place
        avScanV += adc.read(scanVchan);
        avElectronV += adc.read(electronVchan);
        nSamples++;
    }
    else{
        int newT = millis();
        static long oldT;
        // Serial.println(newT - oldT);
        // Serial.print(", ");
        // Serial.println(nSamples);
        oldT = newT;
        if(nSamples) avScanV /= nSamples;
        avScanV -= analogOffset;
        if(nSamples) avElectronV /= nSamples;
        avElectronV -= analogOffset;
        switch(s){
            case Neon:
                floatScanV = 80.0 * float(avScanV) / (4095.0 - analogOffset);
                break;
            case Mercury:
                floatScanV = 30.0 * float(avScanV) / (4095.0 - analogOffset);
                break;
        }    
        floatElectronV = float(avElectronV) / (4095.0 - analogOffset);
        avScanV = 0;
        avElectronV = 0;
        nSamples = 0;
        readingAvailable = true;
    }
}

void sendSerialUpdate(){
    switch(s){
        case Neon:
            Serial.print("N: ");
            break;
        case Mercury:
            Serial.print("M: ");
            break;
    } 
    Serial.print(floatScanV, 3);
    Serial.print(", ");
    Serial.println(floatElectronV, 5);
}

void updateDisplay(){
    if(LCDTimer.timedOut(true) && timeStep >= LCDMinTimeStep){
        char newScanVChar[displayFieldLen];
        char newCurrentChar[displayFieldLen];
        valueToCharArray(newScanVChar, floatScanV, 2);
        valueToCharArray(newCurrentChar, floatElectronV, 4);
        // Serial.print("Old Value: ");
        // Serial.print(oldScanVChar);
        // Serial.print(", New Value: ");
        // Serial.println(newScanVChar);

        // char* np = newScanVChar;
        // char* op = oldScanVChar;
        int lcdOffset = 11;
        // while(*op){
        //     if(*op != *np){
        //         Serial.print(*op);
        //         Serial.print(" to ");
        //         Serial.println(*np);
        //         *op = *np;
        //         lcd.setCursor(lcdOffset, 1);
        //         lcd.print(*np);
        //     }
        //     op++;
        //     np++;
        //     lcdOffset++;
        // }
        for(int i = 0; i < displayFieldLen; i++){
            if(newScanVChar[i] != oldScanVChar[i]){
                // Serial.print(oldScanVChar[i]);
                // Serial.print(" to ");
                // Serial.println(newScanVChar[i]);
                oldScanVChar[i] = newScanVChar[i];
                lcd.setCursor(lcdOffset + i, 1);
                lcd.print(newScanVChar[i]);
            }
        }

        for(int i = 0; i < displayFieldLen; i++){
            if(newCurrentChar[i] != oldCurrentChar[i]){
                // Serial.print(oldCurrentChar[i]);
                // Serial.print(" to ");
                // Serial.println(newCurrentChar[i]);
                oldCurrentChar[i] = newCurrentChar[i];
                lcd.setCursor(lcdOffset + i, 2);
                lcd.print(newCurrentChar[i]);
            }
        }

        // lcd.setCursor(11, 1);
        // lcd.print("      ");
        // lcd.setCursor(11, 1);
        // lcd.print(floatScanV, 2);

        // lcd.setCursor(11, 2);
        // lcd.print("       ");
        // lcd.setCursor(11, 2);
        // lcd.print(floatElectronV, 4);
    }
    if(speciesChanged && debounceTimer.timedOut()){
        setSpecies();
    }
}

void updateTimeValue(uint8_t column, uint8_t row, uint16_t value){
    uint8_t n = timeStepBuffer.update(value);
    for(uint8_t i = 0; i < n; i++){
        lcd.setCursor(column + timeStepBuffer.popIndex(), row);
        lcd.print(timeStepBuffer.popChar());
    }
}

void checkSerial(){
   if(sc.check()){
        if(sc.contains("SA")){
            analogOffset = sc.toInt16();
        }
        else if(sc.contains("SB")){
            bool backlightOn = sc.toInt8();
            if(backlightOn){
                lcd.backlight();
            }
            else{
                lcd.noBacklight();
            }
        }
        else if(sc.contains("ST")){
            timeStep = sc.toInt16();
            dataTimer.updateTimeOut(timeStep);
            updateTimeValue(13, 3, timeStep);
            if(timeStep < LCDMinTimeStep){
                for(int i = 0; i < displayFieldLen - 1; i++){
                    oldCurrentChar[i] = '#';
                    oldScanVChar[i] = '#';
                }
                lcd.setCursor(11, 1);
                lcd.print("#######");
                lcd.setCursor(11, 2);
                lcd.print("#######");
            }
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
            Serial.println("FH"); // for alloy
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

void valueToCharArray(char* charNum, float value, int n_dp){
    char reverseArray[maxLen];
    bool neg = false;
    if(value < 0){
        neg = true;
        value = abs(value);
    }
    if(n_dp != 0){
        value *= pow(10, n_dp);
    }

    value = round(value);

    int intValue = int(value);
    int i = 0;
    while(intValue > 0){
        if((i == n_dp) && (n_dp > 0)){
            reverseArray[i] = '.';
        }
        else{
            reverseArray[i] = (intValue % 10) + '0';
            intValue /= 10;
        }
        i++;
    }
    while(i < n_dp){
        reverseArray[i++] = '0';
    }
    if(i == n_dp){ // if
        if(n_dp > 0){
            reverseArray[i++] = '.';
        }
        reverseArray[i++] = '0';
    }
    if(neg){
        reverseArray[i] = '-';
        i++;
    }
    reverseArray[i] = '\0';
    int len = i;
    for(int i = 0; i < len; i++){
        charNum[i] = reverseArray[len - i - 1];
    }
    for(int i = len; i < maxLen - 1; i++){
        charNum[i] = ' '; // add blank spaces until the end.
    }
    charNum[maxLen - 1] = '\0';

}