#include "DisplayHelper.h"
#include "arduino.h"

DisplayHelper::DisplayHelper(uint8_t arrayLen){
    this->arrayLen = arrayLen; // + 1 to add '\0' to the end
    indices = new uint8_t[arrayLen];
    elements = new char[arrayLen];
    newCharArray = new char[arrayLen + 1];
    buffer = new char[arrayLen + 1];
    for(int i = 0; i < arrayLen; i++){
        indices[i] = 0;
        // elements[i] = ' ';
        // newCharArray[i] = ' ';
        buffer[i] = ' ';
    }
    // elements[arrayLen] = '\0';
    newCharArray[arrayLen] = '\0';
    buffer[arrayLen] = '\0';
    // Serial.print("arrayLen: ");
    // Serial.println(arrayLen);
}
    
DisplayHelper::DisplayHelper(const char* seedCharArray){
    while(*seedCharArray){
        arrayLen++;
    }
    // Serial.print("arrayLen");
    // Serial.println(arrayLen);
    indices = new uint8_t[arrayLen];
    elements = new char[arrayLen];
    newCharArray = new char[arrayLen + 1];
    buffer = new char[arrayLen + 1];
    for(int i = 0; i < arrayLen; i++){
        indices[i] = 0;
        elements[i] = ' ';
        newCharArray[i] = ' ';
        buffer[i] = ' ';
    }
    //elements[arrayLen] = '\0';
    newCharArray[arrayLen] = '\0';
    buffer[arrayLen] = '\0';
}

DisplayHelper::~DisplayHelper(){
    delete[] indices;
    delete[] elements;
}

uint8_t DisplayHelper::set(const char* newBuffer){
    // replaces the contents of the buffer.
    // if the new string is too long, it truncates.
    // if the new string is too short, it pads with spaces
    int i = 0;
    updatesNeeded = 0;
    currentIndex = 0;
    while(newBuffer[i]){
        // buffer[i] = newBuffer[i];
        newCharArray[i] = newBuffer[i];

        i++;
    }
    updatesNeeded = i - 1;
    // Serial.println(i);
    // Serial.println(arrayLen);
    for(i; i < arrayLen; i++){
        // Serial.println(i);
        // buffer[i] = ' ';
        newCharArray[i] = ' ';
    }
    return findUpdatesNeededAndUpdateBuffer(0);
}

char* DisplayHelper::get(){
    return buffer;
}

uint8_t DisplayHelper::update(const char* newCharArray){
    return update(newCharArray, 0);
}

uint8_t DisplayHelper::update(const char* newCharArray, uint8_t startIndex){
    uint8_t i = 0;
    const char* p = newCharArray;
    while(*p){
        this->newCharArray[i] = *p;
        p++;
        i++;
    }
    return findUpdatesNeededAndUpdateBuffer(startIndex);
}

uint8_t DisplayHelper::update(float value, uint8_t n_dp, uint8_t startIndex){
    floatToCharArray(value, n_dp);
    return findUpdatesNeededAndUpdateBuffer(startIndex);
}

uint8_t DisplayHelper::update(float value, uint8_t n_dp){
    floatToCharArray(value, n_dp);
    return findUpdatesNeededAndUpdateBuffer(0);
}

uint8_t DisplayHelper::update(uint8_t value, uint8_t startIndex){
    intToCharArray(value);
    return findUpdatesNeededAndUpdateBuffer(startIndex);
}

uint8_t DisplayHelper::update(uint8_t value){
    intToCharArray(value);
    return findUpdatesNeededAndUpdateBuffer(0);
}

uint8_t DisplayHelper::update(uint16_t value, uint8_t startIndex){
    intToCharArray(value);
    return findUpdatesNeededAndUpdateBuffer(startIndex);
}

uint8_t DisplayHelper::update(uint16_t value){
    intToCharArray(value);
    return findUpdatesNeededAndUpdateBuffer(0);
}

uint8_t DisplayHelper::update(uint32_t value, uint8_t startIndex){
    intToCharArray(value);
    return findUpdatesNeededAndUpdateBuffer(startIndex);
}

uint8_t DisplayHelper::update(uint32_t value){
    intToCharArray(value);
    return findUpdatesNeededAndUpdateBuffer(0);
}

uint8_t DisplayHelper::update(int8_t value, uint8_t startIndex){
    intToCharArray(value);
    return findUpdatesNeededAndUpdateBuffer(startIndex);
}

uint8_t DisplayHelper::update(int8_t value){
    intToCharArray(value);
    return findUpdatesNeededAndUpdateBuffer(0);
}

uint8_t DisplayHelper::update(int16_t value, uint8_t startIndex){
    intToCharArray(value);
    return findUpdatesNeededAndUpdateBuffer(startIndex);
}

uint8_t DisplayHelper::update(int16_t value){
    intToCharArray(value);
    return findUpdatesNeededAndUpdateBuffer(0);
}

uint8_t DisplayHelper::update(int32_t value, uint8_t startIndex){
    intToCharArray(value);
    return findUpdatesNeededAndUpdateBuffer(startIndex);
}

uint8_t DisplayHelper::update(int32_t value){
    intToCharArray(value);
    return findUpdatesNeededAndUpdateBuffer(0);
}

uint8_t* DisplayHelper::getIndices(){
    return indices;
}

char* DisplayHelper::getChars(){
    return elements;
}

uint8_t DisplayHelper::available(){
    return updatesNeeded;
}

uint8_t DisplayHelper::popIndex(){
    return indices[currentIndex];
}

char DisplayHelper::popChar(){
    return elements[currentIndex++];
}

uint8_t DisplayHelper::findUpdatesNeededAndUpdateBuffer(uint8_t startIndex){
    updatesNeeded = 0;
    currentIndex = 0;
    for(int i = startIndex; i < arrayLen - 1; i++){
        if(newCharArray[i - startIndex] != buffer[i]){
            indices[updatesNeeded] = i;
            elements[updatesNeeded] = newCharArray[i - startIndex];
            buffer[i] = newCharArray[i - startIndex];
            updatesNeeded++;
        }
    }
    // updatesNeeded--;
    // elements[updatesNeeded] = '\0';
    // indices[updatesNeeded] = '\0';
    return updatesNeeded;
}

void DisplayHelper::floatToCharArray(float value, int n_dp){
    // char newCharArray[maxLen];
    // Uses Elements array as a temporary array to load the reversed
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
    int len = 0;
    while(intValue > 0){
        if((len == n_dp) && (n_dp > 0)){
            newCharArray[len] = '.';
        }
        else{
            newCharArray[len] = (intValue % 10) + '0';
            intValue /= 10;
        }
        len++;
    }
    while(len < n_dp){
        newCharArray[len++] = '0';
    }
    if(len == n_dp){ // if 
        if(n_dp > 0){
            newCharArray[len++] = '.';
        }
        newCharArray[len++] = '0';
    }
    if(neg){
        newCharArray[len] = '-';
        len++;
    }
    // Now need to reverse the contents so far and pad the rear with blank spaces
    // 5432
    // 0: 2435
    // 1: 2345
    // A 4 digit number takes 2 swaps
    // 54.32
    // 0: 24.35
    // 1: 23.45
    // A 5 digit number also takes 2 swaps. 
    // nSwaps = int(len)/2
    for(int i = 0; i < len/2; i++){
        char tmpC = newCharArray[i];
        newCharArray[i] = newCharArray[len - i - 1];
        newCharArray[len -i - 1] = tmpC;
    }
    for(int i = len; i < arrayLen - 1; i++){
        newCharArray[i] = ' '; // add blank spaces until the end.
    }
}

void DisplayHelper::intToCharArray(uint8_t value){
    int len = 0;
    while(value > 0){
        newCharArray[len] = (value % 10) + '0';
        value /= 10;
        len++;
    }
    // Now reverse the array so it is in the correct order again
    for(int i = 0; i < len/2; i++){
        char tmpC = newCharArray[i];
        newCharArray[i] = newCharArray[len - i - 1];
        newCharArray[len -i - 1] = tmpC;
    }
    for(int i = len; i < arrayLen - 1; i++){
        newCharArray[i] = ' '; // add blank spaces until the end.
    }
}

void DisplayHelper::intToCharArray(uint16_t value){
    int len = 0;
    while(value > 0){
        newCharArray[len] = (value % 10) + '0';
        value /= 10;
        len++;
    }
    // Now reverse the array so it is in the correct order again
    for(int i = 0; i < len/2; i++){
        char tmpC = newCharArray[i];
        newCharArray[i] = newCharArray[len - i - 1];
        newCharArray[len -i - 1] = tmpC;
    }
    for(int i = len; i < arrayLen - 1; i++){
        newCharArray[i] = ' '; // add blank spaces until the end.
    }
}

void DisplayHelper::intToCharArray(uint32_t value){
    int len = 0;
    while(value > 0){
        newCharArray[len] = (value % 10) + '0';
        value /= 10;
        len++;
    }
    // Now reverse the array so it is in the correct order again
    for(int i = 0; i < len/2; i++){
        char tmpC = newCharArray[i];
        newCharArray[i] = newCharArray[len - i - 1];
        newCharArray[len -i - 1] = tmpC;
    }
    for(int i = len; i < arrayLen - 1; i++){
        newCharArray[i] = ' '; // add blank spaces until the end.
    }
}

void DisplayHelper::intToCharArray(int8_t value){
    bool neg = false;
    if(value < 0){
        neg = true;
        value = abs(value);
    }
    int len = 0;
    while(value > 0){
        newCharArray[len] = (value % 10) + '0';
        value /= 10;
        len++;
    }
    if(neg){
        newCharArray[len] = '-';
        len++;
    }
    // Now reverse the array so it is in the correct order again
    for(int i = 0; i < len/2; i++){
        char tmpC = newCharArray[i];
        newCharArray[i] = newCharArray[len - i - 1];
        newCharArray[len -i - 1] = tmpC;
    }
    for(int i = len; i < arrayLen - 1; i++){
        newCharArray[i] = ' '; // add blank spaces until the end.
    }
}

void DisplayHelper::intToCharArray(int16_t value){
    bool neg = false;
    if(value < 0){
        neg = true;
        value = abs(value);
    }
    int len = 0;
    while(value > 0){
        newCharArray[len] = (value % 10) + '0';
        value /= 10;
        len++;
    }
    if(neg){
        newCharArray[len] = '-';
        len++;
    }
    // Now reverse the array so it is in the correct order again
    for(int i = 0; i < len/2; i++){
        char tmpC = newCharArray[i];
        newCharArray[i] = newCharArray[len - i - 1];
        newCharArray[len -i - 1] = tmpC;
    }
    for(int i = len; i < arrayLen - 1; i++){
        newCharArray[i] = ' '; // add blank spaces until the end.
    }
}

void DisplayHelper::intToCharArray(int32_t value){
    bool neg = false;
    if(value < 0){
        neg = true;
        value = abs(value);
    }
    int len = 0;
    while(value > 0){
        newCharArray[len] = (value % 10) + '0';
        value /= 10;
        len++;
    }
    if(neg){
        newCharArray[len] = '-';
        len++;
    }
    // Now reverse the array so it is in the correct order again
    for(int i = 0; i < len/2; i++){
        char tmpC = newCharArray[i];
        newCharArray[i] = newCharArray[len - i - 1];
        newCharArray[len -i - 1] = tmpC;
    }
    for(int i = len; i < arrayLen - 1; i++){
        newCharArray[i] = ' '; // add blank spaces until the end.
    }
}