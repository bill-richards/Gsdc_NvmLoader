#include "Gsdc_NvmLoader.h"

void Gsdc_NvmLoader::displayMemoryContents() 
{ 
  MemoryDataStructure::MemoryDataIterator the_data = _nonValatileMemory->readAll();
  Serial.print("Memory contents : [");
  while(the_data.isValid())
  {
    auto datum = the_data.current_data();
    Serial.print((String)" " + datum->raw());
    the_data.next();
  }
  Serial.println(" ]");
}

String Gsdc_NvmLoader::captureSerialInput()
{
  while(Serial.available() < 1) { delay(100); }
  return Serial.readString();
}

bool Gsdc_NvmLoader::getMessage()
{
  Serial.println("Enter the value you want to store or q|quit to cancel");
  Serial.print("Your Message: ");

  String message = "";
  while(message != "q" && message != "quit" && message.length() == 0)
  {
    message = captureSerialInput();
    message.trim();
  }

  if(message == "q" || message == "quit") { Serial.println(); Serial.println("EXIT"); return false; }
  Serial.println(message);

  String response = "";
  while(response != "y" && response != "yes" && response != "n" &&  response != "no" && response != "q" && response != "quit") 
  {
    Serial.println("Is the message correct?");
    response = captureSerialInput();
  }
  if(response == "q" || response == "quit") { Serial.println("EXIT"); return false; }
  if(response == "n" ||  response == "no") { return getMessage(); }
  
  _receivedMessage = message;
  return true;
}

void Gsdc_NvmLoader::writeMessage(DataMarkers marker) 
{
  if(!getMessage()) { return; }

  Serial.println("Writing " + _receivedMessage + " to memory");
  _nonValatileMemory->write(_receivedMessage, marker);   
}

void Gsdc_NvmLoader::addressNullifiedEventHandler(int address) 
{ 
  if(_quickErase)
    Serial.print(".");
}

void Gsdc_NvmLoader::eepromEraseStartedEventHandler(int memory_size, bool quick_erase) 
{ 
  _quickErase = quick_erase;
  _erasableMemory = memory_size;
  if(quick_erase) 
  {
    Serial.println("Performing a quick erase of EEPROM data");
    return;
  }
  Serial.println("Performing a deep erase of EEPROM data.");
  Serial.println("Please be patient, this can take upwards of five minutes. You will be notified when the operation has completed");
}

void Gsdc_NvmLoader::writeData(char command)
{
  if(command == '1') writeMessage(DataMarkers::BEL);
  if(command == '2') writeMessage(DataMarkers::DC1);
  if(command == '3') writeMessage(DataMarkers::DC2);
  if(command == '4') writeMessage(DataMarkers::DC3);
  if(command == '5') writeMessage(DataMarkers::DC4);
  if(command == '6') writeMessage(DataMarkers::DLE);
  if(command == '7') writeMessage(DataMarkers::ENQ);
  if(command == '8') writeMessage(DataMarkers::ETX);
  if(command == '9') writeMessage(DataMarkers::FF);
  if(command == '0') writeMessage(DataMarkers::FS);
  if(command == 'a') writeMessage(DataMarkers::GS);
  if(command == 's') writeMessage(DataMarkers::SI);
  if(command == 'd') writeMessage(DataMarkers::SO);
  if(command == 'g') writeMessage(DataMarkers::US);
  if(command == 'h') writeMessage(DataMarkers::VT);
}

void Gsdc_NvmLoader::showInstructions() 
{
  Serial.println();
  Serial.println(" Send commands to the serial port:");
  Serial.println(" ---------------------------------");
  Serial.println();
  Serial.println(" 1\t- DataMarkers::BEL");
  Serial.println(" 2\t- DataMarkers::DC1");
  Serial.println(" 3\t- DataMarkers::DC2");
  Serial.println(" 4\t- DataMarkers::DC3");
  Serial.println(" 5\t- DataMarkers::DC4");
  Serial.println(" 6\t- DataMarkers::DLE\t(SSID)");
  Serial.println(" 7\t- DataMarkers::ENQ\t(Remote SSID)");
  Serial.println(" 8\t- DataMarkers::ETX\t(WiFi Password)");
  Serial.println(" 9\t- DataMarkers::FF \t(Remote WiFi Password)");
  Serial.println(" 0\t- DataMarkers::FS");
  Serial.println(" a\t- DataMarkers::GS");
  Serial.println(" s\t- DataMarkers::SI \t(Server Mac Address)");
  Serial.println(" d\t- DataMarkers::SO \t(Remote Server Mac Address)");
  Serial.println(" g\t- DataMarkers::US");
  Serial.println(" h\t- DataMarkers::VT");
  Serial.println(" q\t- Quick erase     \t(Erase to first instance of NUL)");
  Serial.println(" e\t- Full erase      \t(Ensure all addresses are NUL)");
  Serial.println(" c\t- Clear local memory");
  Serial.println(" r\t- Read EEPROM into memory");
  Serial.println(" f\t- Flush");
  Serial.println(" i\t- Show instructions");
  Serial.println("");
}

void Gsdc_NvmLoader::processSerialInput()
{
  if(Serial.available() > 0)
  {
    _serialCommand = Serial.read();

    if((_serialCommand >= '0') && (_serialCommand <= '9') ){ writeData(_serialCommand); }
    if(_serialCommand == 'a'){ writeData(_serialCommand); }
    if(_serialCommand == 's'){ writeData(_serialCommand); }
    if(_serialCommand == 'd'){ writeData(_serialCommand); }
    if(_serialCommand == 'g'){ writeData(_serialCommand); }
    if(_serialCommand == 'h'){ writeData(_serialCommand); }
    
    if(_serialCommand == 'm'){ displayMemoryContents(); }
    if(_serialCommand == 'q'){ _nonValatileMemory->eraseEEPROMData(false); }
    if(_serialCommand == 'e'){ _nonValatileMemory->eraseEEPROMData(true); }
    
    if(_serialCommand == 'c'){ Serial.println("Erasing the in memory data structure"); _nonValatileMemory->clearAll(); }
    if(_serialCommand == 'r'){ Serial.println("Reading the data structure into memory"); _nonValatileMemory->readEEPROM(); }    
    if(_serialCommand == 'f'){ _nonValatileMemory->flush(); }
    if(_serialCommand == 'i'){ showInstructions(); }
  }
}

void Gsdc_NvmLoader::begin()
{
    Serial.println();
    showInstructions();
}

