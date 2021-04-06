#ifndef _Gsdc_NvmLoader_h_
#define _Gsdc_NvmLoader_h_

#include <Arduino.h>
#include <NonVolatileMemory.h>

#define NVM_ON_DEFAULT_I2C_BUS() \
  Gsdc_NvmLoader NvmLoader(0x50);

#define NVM_ON_I2C_BUS(i2c_address) \
  Gsdc_NvmLoader NvmLoader(i2c_address);

#define NVM_BEGIN() NvmLoader.begin();

#define NVM_RUN() NvmLoader.processSerialInput();

class Gsdc_NvmLoader
{
private:
    NonVolatileMemory* _nonValatileMemory;
    String _receivedMessage;
    bool _quickErase = true;
    int _erasableMemory = 0;
    char _serialCommand;

    void showInstructions();
    void displayMemoryContents();
    String captureSerialInput();
    bool getMessage();
    void writeMessage(DataMarkers marker);
    void writeData(char command);
public:
    Gsdc_NvmLoader(int i2c_address)  { _nonValatileMemory = new NonVolatileMemory(i2c_address); }
    ~Gsdc_NvmLoader(){ delete _nonValatileMemory; }
    void begin();
    void processSerialInput();

    void addressNullifiedEventHandler(int address);
    void eepromEraseStartedEventHandler(int memory_size, bool quick_erase);
};

#endif

