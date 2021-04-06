#include <Arduino.h>
#include "Gsdc_NvmLoader.h"

NVM_ON_DEFAULT_I2C_BUS()

void setup() {
  Serial.begin(250000);
  Wire.begin();
  NVM_BEGIN();
}

void loop() 
{
  NVM_RUN()  
  delay(100);
}
