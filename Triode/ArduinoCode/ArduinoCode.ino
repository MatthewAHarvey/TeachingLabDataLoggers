// Tested on Arduino Nano 328.

#include <SPI.h>
#include "MCPDAC.h"
#include "SimpleTimerMAH.h"

//DAC channel mapping:
//DAC0A = X deflectors        Zero volts out at 2071
//DAC0B = Extraction voltage  Zero volts out at 2055


const int numberOfDACChips = 1;
const int numberOfADCs = 4;
int incomingByte = 0;   // for incoming serial data
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
int interval = 2; // how many milliseconds to wait before checking serial port and updating DACs

bool currentPower;
bool previousPower;
MCPDACClass MCPDACs[numberOfDACChips];
byte message[3];
unsigned int dacSelect;
unsigned int dacVoltage = 0;
unsigned int dacVoltages[2 * numberOfDACChips];
unsigned int adcVoltage = 0;

bool updateRequested;
int messageLength;

SimpleTimer dactimer(1); // check for dac updates every 1 ms

// Function prototypes
void initDACs(); //Send switch on condition voltages to DACs. Populate dacVoltages with desired values.
void checkSerial(); //Check for serial messages and respond as appropriate, including updating DACs or reading ADCs and reporting back to PC
void updateDAC(); //Update requested DAC output with new voltage
void readADC(int); //Read the requested analog input and report back to PC

void setup() 
{
  Serial.begin(250000);//115200);
  //setup DACs
  // CS on pin 3 for first DAC
  for (int i=0;i<1;i++){
    dacVoltages[i] = 0;
  }
  initDACs(); 
  Serial.println("Connected");
}

void loop() 
{
  if (dactimer.timedOut(true)){
    checkSerial();
  }
}

void checkSerial(){
  updateRequested=0;
  messageLength=0;
  while(Serial.available() && messageLength<3) {
    // read the incoming byte:
    message[messageLength] = Serial.read();
    messageLength++;
    if(messageLength>=3){updateRequested = 1;}
    //Serial.println("Received");
  }
  if (updateRequested) {
    dacSelect=(unsigned int)message[0]; //if it's a low number, it is a DAC update. If it is a high number (255 down) it is a ADC request.
    dacVoltage=(unsigned int)message[1];//two bytes are sent that need to be combined into an int. Read first byte and convert to int.
    dacVoltage = dacVoltage << 8;       //Next bit shift by 1 byte to the right since the highbyte was sent first and is currently a factor of 2^8=256 too small
    dacVoltage |= (unsigned int)message[2]; //Finally, OR the second (lowerbyte) byte with the shifted combined output int in order to combine them in to the original int value

    if (dacSelect < (2 * numberOfDACChips)) {
      // DAC update has been requested
      updateDAC();
    }
    else if (dacSelect == 255) {
      // All ADC read requested.
      for(int i = 0; i < numberOfADCs; i++){
        message[0] = 255 - (i + 1);
        readADC(i);
      }
    }
    else if ((dacSelect < 255) && (dacSelect >= 255 - numberOfADCs))  {
      // Single ADC read requested. If the number is outside these ranges, ignore request since it must be invalid
      int ADCChannel = 255 - (dacSelect + 1);
      readADC(ADCChannel);
    }
  }
}

void updateDAC(){
  // Write updates to the DACs
  dacVoltages[dacSelect]=dacVoltage; //update dacVoltages array
  int chipNo = dacSelect / 2;
  int chipChan = dacSelect % 2;
  if (chipChan == 0){
    MCPDACs[chipNo].setVoltage(CHANNEL_A,dacVoltage&0x0fff);
  }
  else{
    MCPDACs[chipNo].setVoltage(CHANNEL_B,dacVoltage&0x0fff);
  }
  Serial.write(message,3);
  //Serial.println();
}

void readADC(int ADCChannel){
  // Read the analog inputs and report back to PC
  adcVoltage = analogRead(ADCChannel + 4);
  message[1] = adcVoltage >> 8; // shift 8 bits to the right so that message[1] stores the 8 highest bits.
  message[2] = adcVoltage & 0xff; //bitwise & with 16*16 so that only the lowest 8 bits are stored (f is hexadecimal for 16 so 16*16 = 256 = 2^8) 
  // Now send this message back as 3 serial bytes followed by newline to mark end of message.
  //Serial.println(adcVoltage);
  //Serial.println(message[1]);
  //Serial.println(message[2]);
  //Serial.print(ADCChannel);
  Serial.write(message,3);
  //Serial.println();
}

void initDACs(){
  for (int i = 0; i < numberOfDACChips; i++){
    MCPDACs[i].begin(i+3); //chip select pins are 3,4
    MCPDACs[i].setGain(CHANNEL_A,GAIN_HIGH);  // Set the gain to "HIGH" mode - 0 to 4096mV.
    MCPDACs[i].setGain(CHANNEL_B,GAIN_HIGH);
    MCPDACs[i].shutdown(CHANNEL_A,false);
    MCPDACs[i].shutdown(CHANNEL_B,false);
    // set chips to 2.048V so that opamps are zeroed
    MCPDACs[i].setVoltage(CHANNEL_A,dacVoltages[2*i]);
    MCPDACs[i].setVoltage(CHANNEL_B,dacVoltages[2*i+1]);
  }
}
