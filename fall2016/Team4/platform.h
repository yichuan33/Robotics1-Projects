#include "Math.h"
#include "Arduino.h"

#ifndef PLATFORM_H
#define PLATFORM_H

class Vector{
public:
 float x;
 float y; 
 float z;
 
 Vector(){x=y=z=0;}
 Vector(float x_, float y_, float z_){x=x_, y=y_, z=z_;}

 void setVector(const Vector & vec){
  x = vec.x;
  y = vec.y;
  z = vec.z;
  }
 
 float Length(){
  float a = x*x;
  float b = y*y;
  float c = z*z;

  return sqrt(a+b+c);
  }
};

class Platform{
public:
  Platform();
  void PrintMe(){
    Serial.println("me");
    }
  void TransRot(const Vector &t_vec, const Vector &r_vec);
  
  float getAngles(int n){
      return servo_angles[n];
    }
private:
//platform angles
  float angle_base [6] = {333.5, 26.5, 93.5, 146.5, 213.5, 266.5};
  float angle_platform[6] = {353.5, 6.5, 113.5, 126.5, 233.5, 246.5};
  float angle_beta[6] = {-5*PI/6, 5*PI/6, -PI/6, -PI/2, PI/2, PI/6};

//length measurements
  float height_init = 165; 
  float radius_base = 95; 
  float radius_platform = 61;
  float length_servo_arm = 40;
  float length_rod = 160;

//calculation parameters

  Vector translation;
  Vector rotation;

  Vector joint_base[6];
  Vector joint_platform[6];

  float servo_angles[6];
  Vector overall_leg_vector[6]; //q 
  Vector rod_vector[6]; //l
  Vector servo_arm_point[6]; //A

void calcLegPos();
void calcServoAngles();

};

#endif
