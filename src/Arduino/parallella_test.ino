#include <GY_85.h>
#include <Wire.h>

GY_85 GY85;
float gx[3];
float gy[3];
float gz[3];
float x;
float y;
float z;
int time;
int counter;
float cal = 1.37;

struct pack {
  byte code = 0x00;
  int num = 0;
  float x = 0;
  float y = 0;
  float z = 0;
  byte c = 0;
};

pack* p = (pack*) malloc(sizeof(pack));

void setup() {
  Wire.begin(); 
  delay(10);
  Serial.begin(9600);
  Serial3.begin(115200);
  delay(10);
  GY85.init();
  
  int i=0;
  for (i=0; i<3; i++) {
    gx[i] = 0.0;
    gy[i] = 0.0;
    gz[i] = 0.0;
  }
  
  x = 0.0;
  y = 0.0;
  z = 0.0;
  
  time = 20;
  counter = 0;
  
}

float gxa, gya, gza;

void loop() {
  
  //int cx = GY85.compass_x( GY85.readFromCompass() );
  //int cy = GY85.compass_y( GY85.readFromCompass() );
  //int cz = GY85.compass_z( GY85.readFromCompass() );
  gx[2] = gx[1];
  gy[2] = gy[1];
  gz[2] = gz[1];
  
  gx[1] = gx[0];
  gy[1] = gy[0];
  gz[1] = gz[0];
  
  gx[0] = GY85.gyro_x( GY85.readGyro() );
  gy[0] = GY85.gyro_y( GY85.readGyro() );
  gz[0] = GY85.gyro_z( GY85.readGyro() );
  
  gxa = (gx[2] + gx[1] + gx[0])/3;
  gya = (gy[2] + gy[1] + gy[0])/3;
  gza = (gz[2] + gz[1] + gz[0])/3;
  
  float val = gxa/1000.0 * ((float)time) * cal;
  if (abs(val) > 0.07)
    x += val;
   
  val = gya/1000.0 * ((float)time) * cal;
  if (abs(val) > 0.07)
    y += val;
   
  val = gza/1000.0 * ((float)time) * cal;
  if (abs(val) > 0.07)
    z += val;
  
  //Serial.print(cx);
  //Serial.print(" y: ");
  //Serial.print(cy);
  //Serial.print(" z: ");
  //Serial.println(cz);
  
  //if (Serial3.available()) {
    //char c = Serial3.read();
    Serial3.print("x "); 
    Serial3.print(x);
    Serial3.print(" y ");
    Serial3.print(y);
    Serial3.print(" z ");
    Serial3.println(z);
  //}


  
  delay(time);
}

