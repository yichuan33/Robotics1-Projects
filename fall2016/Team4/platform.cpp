#include "Math.h"
#include "Arduino.h"
#include "platform.h"

Vector subtract(Vector &q1, Vector &q2);
Vector sum(Vector &q1, Vector &q2);
float Square(Vector &vec);

Platform::Platform(){
  Serial.println("here I am!");

  translation = Vector();
  rotation = Vector();


  for(int i=0; i<6; ++i){
  	float b_joint_x = radius_base*cos(radians(angle_base[i]));
  	float b_joint_y = radius_base*sin(radians(angle_base[i]));
  	joint_base[i] = Vector(b_joint_x, b_joint_y, 0);
  }

   for(int i=0; i<6; ++i){
  	float p_joint_x = radius_platform * cos(radians(angle_platform[i]));
  	float p_joint_y = radius_platform * sin(radians(angle_platform[i]));
  	joint_platform[i] = Vector(p_joint_x, p_joint_y, 0);

  	overall_leg_vector[i] = Vector();
  	rod_vector[i] = Vector();
  	servo_arm_point[i] = Vector();
  }
  calcLegPos();
}

void Platform::TransRot(const Vector &t_vec, const Vector &r_vec){
  //Serial.println("applied");
	rotation.setVector(r_vec);
	translation.setVector(t_vec);
  //Serial.println(t_vec.z);
  calcLegPos();
  calcServoAngles();
}

void Platform::calcLegPos(){
	for (int i=0; i<6; i++) {
      // rotation
      overall_leg_vector[i].x = 	
      			cos(rotation.z)		*cos(rotation.y)	*joint_platform[i].x +
        		(-sin(rotation.z)	*cos(rotation.x)	+cos(rotation.z)	*sin(rotation.y)*sin(rotation.x))*joint_platform[i].y +
        		(sin(rotation.z)	*sin(rotation.x)	+cos(rotation.z)	*sin(rotation.y)*cos(rotation.x))*joint_platform[i].z;

      overall_leg_vector[i].y = 	
      			sin(rotation.z)		*cos(rotation.y)	*joint_platform[i].x +
       			(cos(rotation.z)	*cos(rotation.x)	+sin(rotation.z)	*sin(rotation.y)*sin(rotation.x))*joint_platform[i].y +
        		(-cos(rotation.z)	*sin(rotation.x)	+sin(rotation.z)	*sin(rotation.y)*cos(rotation.x))*joint_platform[i].z;

      overall_leg_vector[i].z = 	
      			-sin(rotation.y)	*joint_platform[i].x +
        		cos(rotation.y)		*sin(rotation.x)	*joint_platform[i].y +
        		cos(rotation.y)		*cos(rotation.x)	*joint_platform[i].z;

      // translation
      overall_leg_vector[i] = sum(overall_leg_vector[i], translation);
      overall_leg_vector[i].z += height_init;
      rod_vector[i] = subtract(overall_leg_vector[i], joint_base[i]);

    }
}

void Platform::calcServoAngles(){
	   for (int i=0; i<6; i++) {
      	int L = Square(rod_vector[i])-(length_rod*length_rod)+(length_servo_arm*length_servo_arm);
      	float M = 2*length_servo_arm*(overall_leg_vector[i].z-joint_base[i].z);
      	float N = 2*length_servo_arm*(cos(angle_beta[i])*(overall_leg_vector[i].x-joint_base[i].x) + sin(angle_beta[i])*(overall_leg_vector[i].y-joint_base[i].y));

      	/*Serial.print(i);
      	Serial.print(" L ");
        Serial.print(L);
        Serial.print(" M ");
        Serial.print(M);
        Serial.print(" N ");
        Serial.println(N);*/
      	servo_angles[i] = asin(L/sqrt(M*M+N*N)) - atan2(N, M);
/*
      	A[i].set(hornLength*cos(alpha[i])*cos(beta[i]) + baseJoint[i].x, 
      	hornLength*cos(alpha[i])*sin(beta[i]) + baseJoint[i].y, 
      	hornLength*sin(alpha[i]) + baseJoint[i].z);

      	float xqxb = (q[i].x-baseJoint[i].x);
      	float yqyb = (q[i].y-baseJoint[i].y);
      	float h0 = sqrt((legLength*legLength)+(hornLength*hornLength)-(xqxb*xqxb)-(yqyb*yqyb)) - q[i].z;

      	float L0 = 2*hornLength*hornLength;
      	float M0 = 2*hornLength*(h0+q[i].z);
      	float a0 = asin(L0/sqrt(M0*M0+N*N)) - atan2(N, M0);
*/
/*
        Serial.print("angle ");
        Serial.print(i);
        Serial.print(" ");
      	Serial.println(servo_angles[i]*180/PI);*/
    }
}

Vector subtract(Vector &q1, Vector &q2){
	float a = q1.x - q2.x;
	float b = q1.y - q2.y;
	float c = q1.z - q2.z;
	return Vector(a, b, c);
}

Vector sum(Vector &q1, Vector &q2){
	float a = q1.x + q2.x;
	float b = q1.y + q2.y;
	float c = q1.z + q2.z;
	return Vector(a, b, c);
}

float Square(Vector &vec){
	int a = (vec.x)*(vec.x);
	int b = (vec.y)*(vec.y);
	int c = (vec.z)*(vec.z);
	return a+b+c;
}
