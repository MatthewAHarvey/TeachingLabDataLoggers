// Display Helper
// Creates a char array buffer and given a new c-style char array or number, returns which elements on the screen need updating and what they are. 
#include "arduino.h"
#ifndef DisplayHelper_h
#define DisplayHelper_h

class DisplayHelper{
public:
    DisplayHelper(uint8_t arrayLen);
    DisplayHelper(const char* seedCharArray);
    ~DisplayHelper();

    uint8_t set(const char* newBuffer);
    char* get();
    // can I return a struct with the number of elements, an index array and a char array? 
    uint8_t update(const char* newCharArray);
    uint8_t update(const char* newCharArray, uint8_t startIndex); // updates the char array and returns the number of changed elements
    uint8_t update(float value, uint8_t n_dp);
    uint8_t update(float value, uint8_t n_dp, uint8_t startIndex);
    uint8_t update(uint8_t value);
    uint8_t update(uint16_t value);
    uint8_t update(uint32_t value);
    uint8_t update(uint8_t value, uint8_t startIndex);
    uint8_t update(uint16_t value, uint8_t startIndex);
    uint8_t update(uint32_t value, uint8_t startIndex);
    uint8_t update(int8_t value);
    uint8_t update(int16_t value);
    uint8_t update(int32_t value);
    uint8_t update(int8_t value, uint8_t startIndex);
    uint8_t update(int16_t value, uint8_t startIndex);
    uint8_t update(int32_t value, uint8_t startIndex);
    uint8_t* getIndices(); // returns the positions of the changes that need to be made
    char* getChars(); // the update chars array
    uint8_t available();
    uint8_t popIndex();
    char popChar();
private:
    uint8_t findUpdatesNeededAndUpdateBuffer(uint8_t startIndex);
    void floatToCharArray(float value, int n_dp);
    void intToCharArray(uint8_t value);
    void intToCharArray(uint16_t value);
    void intToCharArray(uint32_t value);
    void intToCharArray(int8_t value);
    void intToCharArray(int16_t value);
    void intToCharArray(int32_t value);

    uint8_t currentIndex = 0;
    uint8_t arrayLen = 0;
    uint8_t updatesNeeded = 0;
    uint8_t* indices = NULL;
    char* elements = NULL;
    char* newCharArray = NULL;
    char* buffer = NULL;
};

#endif

