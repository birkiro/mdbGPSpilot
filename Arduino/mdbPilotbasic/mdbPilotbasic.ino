#include <TinyGPS.h>
#include <SoftwareSerial.h>

SoftwareSerial GPS = SoftwareSerial(4,3);
TinyGPS        gps;
unsigned long fix_age = 0;

float lat_home, lon_home;
float lat_wp, lon_wp;
int flag_flying = 0;
int  fix_led = 13;
float flat, flon;
unsigned long age;

void setup()
{
  bool fixed = false;
  Serial.begin(115200);
  GPS.begin(9600);
  pilot_setup();
}

int gps_counter = 0;
int acquired = 0;
int i = 1;

void loop()
{ 
  if (GPS.available()) 
  {
     int byte = GPS.read();
     //Serial.write(byte); 
     if(gps.encode(byte)) 
     {
       gps.f_get_position(&flat, &flon, &fix_age);
       if ( gps_counter++ % 2 == 0) acquired = 1;
     }
    
  }
  //Serial.println(acquired);
  if(acquired)
  {
    drone_sendpos();
    
    Serial.print("$ECHO,#Setting Home Position and Waypoint \n");
    while(i > 0)
    {
      drone_sethomepos();  // Set Home Position, set global variables lat_home, lon_home
//      drone_setwp(54.900700, 9.807400);   // Set Waypoint Position (longitude, latitude)
      drone_setwp(54.913328, 9.779755);   // Set Waypoint Position (longitude, latitude)
      Serial.print("Distance to wp:");
      Serial.println(calculate_distance(lat_home, lon_home, lat_wp, lon_wp));
      while(calculate_distance(lat_home, lon_home, lat_wp, lon_wp) > 500) //Stay in loop when distance>500m
      {
        Serial.print("$ECHO,#Distance too great! Flight Aborted! \n");
        Serial.print("Distance to wp:");
        Serial.println(calculate_distance(lat_home, lon_home, lat_wp, lon_wp));
        delay(1000);
      }
      drone_takeoff();
      Serial.print("$ECHO,#We Have Liftoff! \n");
      i--;
    }
    
    float angle = gps.course_to (flat, flon, lat_wp, lon_wp) * 1000; // $HEAD need 1/1000 of degree
    float distance_to_wp = gps.distance_between(flat, flon, lat_wp, lon_wp);
    
    head_to_target(angle);
    drone_move(distance_to_wp);
    
    if (distance_to_wp < 5) 
    {
      drone_hove();
      delay(5000);
      drone_land();
    }
    acquired = 0;
  } 
}



void pilot_setup()
{ // Executes the binary on AR.Drone 2.0, the arpilot code has to be compiled for arm architecture
  Serial.print("\n\n\n\n\n$QUIT\n\n");
  delay(1000);
  Serial.print("cat - | ./data/video/arpilot\n");      //no more USB, binary file in /data/video folder
  delay(3000);
  Serial.print("cat - | ./data/video/arpilot\n");      // Send it once more for safety
  delay(1000);

  /*Limits: max_yaw 2.50
             max_vz  700
          max_euler 0.30
           altitude 50000 (50m)*/
  Serial.print("$LIMI,250,700,30,50000\n"); /*Not sure that this works yet*/
}

void drone_sethomepos()
{
  lat_home = flat;
  lon_home = flon;
//  Serial.print("Home Coordinates: ");
//  Serial.print(flat);
//  Serial.print(", ");
//  Serial.println(flon);
}

void drone_sendpos()
{ // Update Drone executable, commands sent as SDTDIN.
  Serial.print("$PLON,");
  Serial.print(flon,10);
  Serial.print("\n");
  
  Serial.print("$PLAT,");
  Serial.print(flat,10);
  Serial.print("\n");

  //Serial.print("$PCOU,");
  //Serial.print(course_b,10); // Heading is calculated knowing current position and waypoint position
  //Serial.print("\n");
  
//  Serial.print("$HDOP,");
//  Serial.print(gps.hdop());
//  Serial.print("\n");
}

void drone_setwp(float longitude, float latitude)
{
  lat_wp = longitude;
  lon_wp = latitude;
}

float calculate_distance(float lat_home, float lon_home, float lat_wp, float lon_wp)
{
  return gps.distance_between(lat_home, lon_home, lat_wp, lon_wp);
}


float calculate_heading(float target_waypoint)
{
  float heading;
  
  // Calculate it
  return heading;
}
 
void drone_takeoff()
{
  Serial.print("$STAT,1\n");
  delay(6000);
  Serial.print("$CALI\n");
  delay(6000);
  Serial.print("$ALTI,1500,0\n");
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
  int kp = (-50);
  float tpitch = dist_to_wp * kp;
//  Serial.print("$ECHO,#");
//  Serial.println(tpitch);
  if(tpitch > 0) tpitch = 0;
  if(tpitch < -1024) tpitch = -1024;
  
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
  Serial.print("$ALTI,1500,0\n");
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

