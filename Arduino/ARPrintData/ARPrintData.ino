#include <SoftwareSerial.h>

void pilot_setup()
{
//  delay(1500);
//  Serial.print("cat - | /data/video/usb/arpilot > /data/video/usb/plog.txt\n");
//  delay(10000);
//  Serial.print("cat - | /data/video/usb/arpilot > /data/video/usb/plog.txt\n");
//
//  delay(2000);
}

void setup()
{
  Serial.begin(115200);
  
  //pilot_setup();
}

void loop()
{
//    delay(1000);
//    Serial.print("$PRIN");
//    Serial.print("\n");
//    delay(1000);
//    
    delay(2000);
    Serial.print("$STAT,1");
    Serial.print("\n");
    delay(4000);
    Serial.print("$STAT,0");
    Serial.print("\n");
    delay(2000);
    return;
}

