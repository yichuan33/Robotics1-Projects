#include "platform.h"
#include <Servo.h>


int base_angle = 68;
int A1_offset=0;
int A2_offset=0;
int B1_offset=-3;
int B2_offset=-6;
int C1_offset=6;
int C2_offset=1;

Servo a1;
Servo a2;
Servo b1;
Servo b2;
Servo c1;
Servo c2;


Platform platform;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  a1.attach(3);
  a2.attach(5);
  b1.attach(6);
  b2.attach(9);
  c1.attach(10);
  c2.attach(11);
}

void loop() {
#if 1
  TransDemo();
  RotDemo();
#endif

#if 0
  MoveTo(500, 580);
#endif
}

void WriteToServo(int angles[]){
  /*
  int a1_val =         base_angle + A1_offset + angles[0];
  int a2_val = 180 - ( base_angle + A2_offset + angles[1]);
  int b1_val =         base_angle + B1_offset + angles[2];
  int b2_val = 180 - ( base_angle + B2_offset + angles[3]);
  int c1_val =         base_angle + C1_offset + angles[4];
  int c2_val = 180 - ( base_angle + C2_offset + angles[5]);
  */

  int a1_val =       ( base_angle + A1_offset + angles[0]);
  int a2_val = 180 - ( base_angle + A2_offset + angles[1]);
  int b1_val =       ( base_angle + B1_offset + angles[2]);
  int b2_val = 180 - ( base_angle + B2_offset + angles[3]);
  int c1_val =       ( base_angle + C1_offset + angles[4]);
  int c2_val = 180 - ( base_angle + C2_offset + angles[5]);

  a1.write(a1_val);
  a2.write(a2_val);
  b1.write(b1_val);
  b2.write(b2_val);
  c1.write(c1_val);
  c2.write(c2_val);
  }

  void Move(Vector trans, Vector rot){
    platform.TransRot(trans, rot);
    int angles[6];
      for(int i=0; i<6; ++i){
        angles[i] = (int)(platform.getAngles(i)*180/PI);
      }
      WriteToServo(angles);
    }

 void MoveTo(int x, int y){
  int err_x = 0;
  int err_y = 0;
  int err_x_prev = 0;
  int err_y_prev = 0;
  int d_err_x = 0;
  int d_err_y = 0;
  
  while(1){

  err_x_prev = err_x;
  err_y_prev = err_y;
  
  int current_x = readX();
  int current_y = readY();
  err_x = x - current_x;
  err_y = y - current_y;
  
  d_err_x = err_x - err_x_prev;
  d_err_y = err_y - err_y_prev;

  float kp = 0.0004;
  float kd = 0.008;
  
  float rotx = -kp * err_x - kd * d_err_x;
  float roty = -kp * err_y - kd * d_err_y;  
  
  Move(Vector(), Vector(rotx, roty, 0));
  //delay(10);
    }
   
 }

  void TransDemo(){
    Vector translation;
    Vector rotation;
    translation.x+=30;
    Move(translation, rotation);
    delay(500);
    translation.x-=30;
    Move(translation, rotation);
    delay(500);

    translation.y+=30;
    Move(translation, rotation);
    delay(500);
    translation.y-=30;
    Move(translation, rotation);
    delay(500);

    translation.z+=30;
    Move(translation, rotation);
    delay(500);
    translation.z-=30;
    Move(translation, rotation);
    delay(500);

  }

  void RotDemo(){
    Vector translation;
    Vector rotation;
    rotation.x+=0.3;
    Move(translation, rotation);
    delay(500);
    rotation.x-=0.3;
    Move(translation, rotation);
    delay(500);

    rotation.y+=0.3;
    Move(translation, rotation);
    delay(500);
    rotation.y-=0.3;
    Move(translation, rotation);
    delay(500);

    rotation.z+=0.6;
    Move(translation, rotation);
    delay(500);
    rotation.z-=0.6;
    Move(translation, rotation);
    delay(500);

  }
//A0 --> X1
//A1 --> X2
//A3 --> Y1
//A4 --> Y2
 int readX(){
    pinMode(A1, INPUT);
    pinMode(A3, INPUT);

    pinMode(A0, OUTPUT);
    digitalWrite(A0, HIGH);
    pinMode(A2, OUTPUT);
    digitalWrite(A2, LOW);
    return analogRead(A1);
    //Serial.println(X);
  }
 int readY(){
    pinMode(A0, INPUT);
    pinMode(A2, INPUT);

    pinMode(A1, OUTPUT);
    digitalWrite(A1, HIGH);
    pinMode(A3, OUTPUT);
    digitalWrite(A3, LOW);
    return analogRead(A0);
    //Serial.println(X);
  }
