#include <SoftwareSerial.h>

SoftwareSerial GPS = SoftwareSerial(4,3);

int flag_flying = 0;

void setup()
{
  Serial.begin(115200);
  GPS.begin(9600);
  pilot_setup();
}

int gps_counter = 0;
int acquired = 0;
int i = 1;

void loop()
{ 
  while(i > 0)
  {
    drone_takeoff();
    i--;
  }
  drone_hove();
  drone_land();
}

void pilot_setup()
{ // Executes the binary on AR.Drone 2.0, the arpilot code has to be compiled for arm architecture
  Serial.print("\n\n\n\n\n$QUIT\n\n");
  delay(1000);
  Serial.print("cat - | ./data/video/arpilot\n");      //no more USB, binary file in /data/video folder
  delay(4000);
  Serial.print("cat - | ./data/video/arpilot\n");      // Send it once more for safety
  delay(4000);
  Serial.print("$LIMI,250,700,30,50000\n"); /*Not sure that this works yet*/
}
 
void drone_takeoff()
{
  Serial.print("$STAT,1\n");
  delay(10000);
  Serial.print("$HOVE\n");
  delay(5000);
  Serial.print("$CALI\n");
  delay(6000);
  Serial.print("$ALTI,3000,1\n");
  delay(3000);
  flag_flying = 1;
}

void head_to_target(float angle)
{
   Serial.print("$HEAD,");
   Serial.print(angle,0);
   Serial.print("\n");
   //delay(500);
}

void drone_move(float dist_to_wp)
{
//  Serial.print("$ECHO,#");
//  Serial.println(dist_to_wp);
  int kp = (-10);
  float tpitch = dist_to_wp * kp;
//  Serial.print("$ECHO,#");
//  Serial.println(tpitch);
  if(tpitch > 0) tpitch = 0;
  if(tpitch < -200) tpitch = -200;
  
//  Serial.print("$ECHO,#");
//  Serial.println(tpitch);
  Serial.print("$MOVE,");
  Serial.print(0);
  Serial.print(",");
  Serial.print(tpitch,0);
  Serial.print(",");
  Serial.print(0);
  Serial.print(",");
  Serial.print(0);
  Serial.print("\n");
  delay(200);
  Serial.print("$ALTI,3000,1\n");
}

void drone_hove()
{
  Serial.print("$HOVE");
  Serial.print("\n");
}

void drone_land()
{
  Serial.print("$STAT,0");
  Serial.print("\n");
  flag_flying = 0;
}

