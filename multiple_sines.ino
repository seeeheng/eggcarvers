#include <Servo.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Servo myServo;
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);


int azimuth = 0;
int elevation = 0;
int azimuth_offset = 0;
int elevation_offset = 0;
int dx = 1;
int dy = 1;

void setup() {
  Serial.begin(9600);
  myServo.attach(9);
  //myservo2.attach(10);
  //myservo1.write(azimuth_offset);
  //myservo2.write(elevation_offset);

  Serial.println("Stepper test!");

  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  
  myMotor->setSpeed(10);  // 10 rpm   
}

//servo middle:55
void loop() {
  //delay(5000);
  //myServo.write(55);
  //drawSineWave();
  //delay(2000);
  //myServo.write(50);
  //delay(2000);
  //myServo.write(60);
  //drawLine(0, 90, 0, 20);
  delay(10000);
  
  drawBezier(357, 746, 94, 543, 48, 358, 157, 358);
  delay(1000);
  drawBezier(157, 358, 265, 358, 354, 460, 354, 460);
  delay(1000);
  drawBezier(354, 461, 354, 460, 391, 372, 480, 369);
  delay(1000);
  drawBezier(480, 369, 568, 366, 494, 678, 357, 746);
  
  //drawRect(0, 20, 50, 90);
  //drawLine(0, 40, 100, 80);
  delay(2000);
  //drawLine(0, 20, 0, 100);
  //delay(5000);
  //drawEllipse(0, 0, 60, 30);
}

/*void move(int dx, int y) {
  myMotor->step(dx, FORWARD, MICROSTEP);
  myServo.write(y);
}*/

void drawSineWave() {
  double x = 0;
  while(true) {
    myMotor->step(1, FORWARD, MICROSTEP);
    x += (360.0 / 200);
    myServo.write(55 + 40 * sin(2 * x / 180.0 * 3.14159));
  }
}

void drawLine(int x1, int y1, int x2, int y2) {
  // draw from (x1, y1) to (x2, y2);

  
  int ydir = 1;
  if (y2 < y1) {
    ydir = -1;
  }
  // if line is vertical
  if (x2 == x1) {
     for (int i = 0; i < abs(y1 - y2); i += dy) {
        myServo.write(y1 + ydir * i);
        delay(50);
     }
  }
  else {
    double slope = ((y2 - y1) * 1.0) / (x2 - x1);
    for (int i = 0; i < abs(x2 - x1); i += dx) {
        if (x2 > x1) {
          myMotor->step(dx, FORWARD, MICROSTEP);
        }
        else {
          myMotor->step(dx, BACKWARD, MICROSTEP);
        }
        myServo.write(y1 + ydir * i * slope);
        Serial.println(y1 + ydir * i * slope);
     }
  }
}


void drawRect(int cx, int cy, int width, int height) {
  drawLine(cx, cy, cx + width, cy);
  drawLine(cx + width, cy, cx + width, cy + height);
  drawLine(cx + width, cy + height, cx, cy + height);
  drawLine(cx, cy + height, cx, cy);
}

void drawBezier(int p0x, int p0y, int p1x, int p1y, int p2x, int p2y, int p3x, int p3y) {
  double last_x = 0.0;
  for (double t = 0; t < 1.0; t += 0.001) {
    //Serial.println(t);
    double x = pow((1-t),3)*p0x + 3*t*pow((1-t), 2)*p1x + 3*pow(t,2)*(1-t)*p2x + pow(t,3) * p3x;
    double y = pow((1-t),3)*p0y + 3*t*pow((1-t), 2)*p1y + 3*pow(t,2)*(1-t)*p2y + pow(t,3) * p3y;
    if ((x - last_x) > 10) {
      Serial.println("MOVING!");
      myMotor->step(1, FORWARD, MICROSTEP);
      myServo.write(round(y / 6) - 30);
      last_x = x;
    }
    else if ((last_x - x) > 10) {
      Serial.println("MOVING!");
      myMotor->step(1, BACKWARD, MICROSTEP);
      myServo.write(round(y / 6) - 30);
      last_x = x;
    }
  }
}

/*void drawEllipse(int cx, int cy, int rx, int ry) {
  // this is going to try parametric graphing

  double prevx = rx * cos(0) + cx;
  double prevy = ry * sin(0) + cy;
  for (int i = 0; i < 360; i++) {
    double t = i / 180 * 3.14159;
    int x = (int) (rx * cos(t) + cx);
    int y = (int) (ry * sin(t) + cy); 
    double dx = (prevx - x);
    double dy = (prevy - y);
    double factor = 1 / dx;
    dy = dy * factor;
  }
}*/



/*
void drawPolyLine(int xs[], int ys[]) {
    moveTo(xs[0], ys[0], false);
    for (int i = 0; i < sizeof(xs); i += 2) {
      moveTo(xs[i], ys[i], true);
    }
}

void drawBezier(int mx, int my, int sx, int sy, int ex, int ey, int fx, int fy) {
   //moveTo(mx, my, false);
}

void drawEllipse(int cx, int cy, int rx, int ry) {
  // this is going to try parametric graphing
  
  for (int i = 0; i < 360; i++) {
    double t = i / 180 * 3.14159;
    int x = (int) (rx * cos(t) + cx);
    int y = (int) (ry * sin(t) + cy); 
  }
}

int transform(int x, int y) {
  // this function will apply a Galls-Peter transformation
  return 0;
}*/

