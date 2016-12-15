#include <Servo.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Servo pen;
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_StepperMotor *rotationMotor = AFMS.getStepper(200, 2); // Rotate
Adafruit_StepperMotor *elevationMotor = AFMS.getStepper(200, 1); // Elevate


// rotation range: 0 - 200 (wrap around)
// elevation range: 0 - 200 (no wrap around)
double x_calibration = 1;
double y_calibration = 1;
int x_step = 1;
int y_step = 4;
int pen_down = 125;
int pen_up = 180;
int precision = 20;
int ymax = 200;
int calibration[20];
const double pi = 3.141596;

int azimuth = 0;
int elevation = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting!");
  delay(2000);
  pen.attach(9);
  pinMode(7, OUTPUT);
  pinMode(6, INPUT);

  AFMS.begin();  // create with the default frequency 1.6KHz
}

void loop() { 
  calibratePen();
  delay(2000);
  Serial.println("Calibration has been finished!");
  // Center is 100, 120 
  
  moveTo(90, 60);
  drawRect(90, 60, 20, 90); // body 20x80

  moveTo(95, 100);
  drawLine(95, 100, 75, 120, true);
  drawLine(75, 120, 75, 100, true); // arm
  drawLine(75, 100, 95, 80, true); 

  moveTo(105, 100);
  drawLine(105, 100, 125, 120, true);
  drawLine(125, 120, 125, 100, true); // right arm
  drawLine(125, 100, 105, 80, true);

  moveTo(90, 60);
  drawLine(90, 60, 80, 30, true);
  drawLine(80, 30, 90, 30, true);
  drawLine(90, 30, 100, 60, true);
  drawLine(100, 60, 110, 30, true); // legs
  drawLine(110, 30, 120, 30, true);
  drawLine(120, 30, 110, 60, true);
  
  moveTo(90, 160);
  drawBezier(90, 160, 95, 210, 105, 210, 110, 160); // head
  drawBezier(110, 160, 105, 110, 95, 110, 90, 160);

  delay (5000);
  Serial.println("Done drawing!");
}

void calibratePen() {
  int offset = 13;
  digitalWrite(7, HIGH);
  int sensorRead = digitalRead(6);
  
  for (int i = 0; i < precision; i++) {
    pen.write(180);
    moveTo(0, i * ymax / precision);

    int depth = 180;
    while (sensorRead == LOW) {
      digitalWrite(7, HIGH);
      sensorRead = digitalRead(6);
      pen.write(depth);
      depth--;
      delay(10);
    }
    calibration[i] = depth + offset;
    Serial.print("Latitude ");
    Serial.print(i * ymax / precision);
    Serial.print(": ");
    Serial.println(depth);
    pen.write(180);
    delay(200);
    digitalWrite(7, HIGH);
    sensorRead = digitalRead(6);
    delay(200);
  }
}

void adjustPen(bool draw) {
  if (!draw) {
    pen.write(180);
    return;
  }
  for (int i = 0; i < precision; i++) {
    if (elevation < i * ymax / precision) {
      pen.write(calibration[i]);
      return;
    }
  }
}


void moveTo(int startx, int starty) {
  if (azimuth == startx && elevation == starty) {
    return;
  }
  drawLine(azimuth, elevation, startx, starty, false);
}


// startx, starty, endx, endy
// use this but set "bool draw" to false if you just want to move between points but not draw
void drawLine(int startx, int starty, int endx, int endy, bool draw) {  
  moveTo(startx, starty);
  
  if (startx == endx) {
    // vertical line
    if (endy > starty) {
      for (int y = starty; y < endy; y += y_calibration) {
        elevationMotor->step(y_step, FORWARD, SINGLE);
        elevation += 1;
        adjustPen(draw);
      }
    }
    else {
      for (int y = starty; y > endy; y -= y_calibration) {
        elevationMotor->step(y_step, BACKWARD, SINGLE);
        elevation -= 1;
        adjustPen(draw);
      }
    }
  }

  else {
    double slope = (1.0 * endy - starty) / (1.0 * endx - startx);
    double intercept = slope * -startx + starty; 
    double last_x = 0.0;
    double last_y = 0.0;

    for (double t = 0.0; t <= 1.0; t += 0.0001) {
      double x = (endx - startx) * t;
      double y = slope * x + intercept;

      if ((x - last_x) > x_calibration) {
        rotationMotor->step(x_step, FORWARD, SINGLE);
        last_x = x;
        azimuth += 1;
      }
      else if ((last_x - x) > x_calibration) {
        rotationMotor->step(x_step, BACKWARD, SINGLE);
        last_x = x;
        azimuth -= 1;
      }
      if ((y - last_y) > y_calibration) {
        elevationMotor->step(y_step, FORWARD, SINGLE);
        last_y = y;
        elevation++;
        adjustPen(draw);
      }
      else if ((last_y - y) > y_calibration) {
        elevationMotor->step(y_step, BACKWARD, SINGLE);
        last_y = y;
        elevation--;
        adjustPen(draw);
      }
    }
  }
  azimuth = endx;
  elevation = endy;
}


void drawRect(int cx, int cy, int width, int height) {
  moveTo(cx, cy);
  drawLine(cx, cy, cx + width, cy, true);
  drawLine(cx + width, cy, cx + width, cy + height, true);
  drawLine(cx + width, cy + height, cx, cy + height, true);
  drawLine(cx, cy + height, cx, cy, true);
}

void drawBezier(int p0x, int p0y, int p1x, int p1y, int p2x, int p2y, int p3x, int p3y) {
  moveTo(p0x, p0y);
  adjustPen(true);

  double numpoints = 0.0001;
  /*if (abs(p3x - p0x) < 10 && abs(p3y - p0y) < 10) {
    numpoints = 0.001;
  }*/
  double last_x = 0.0;
  double last_y = 0.0;
  for (double t = 0.0; t <= 1.0; t += numpoints) {
    double x = pow((1.0-t),3)*p0x + 3*t*pow((1.0-t), 2)*p1x + 3*pow(t,2)*(1-t)*p2x + pow(t,3) * p3x;
    double y = pow((1.0-t),3)*p0y + 3*t*pow((1.0-t), 2)*p1y + 3*pow(t,2)*(1-t)*p2y + pow(t,3) * p3y;
    if ((x - last_x) > x_calibration) {
      rotationMotor->step(x_step, FORWARD, SINGLE);
      last_x = x;
      azimuth += 1;
    }
    else if ((last_x - x) > x_calibration) {
      rotationMotor->step(x_step, BACKWARD, SINGLE);
      last_x = x;
      azimuth -= 1;
    }
    if ((y - last_y) > y_calibration) {
      elevationMotor->step(y_step, FORWARD, SINGLE);
      last_y = y;
      elevation += 1;
      adjustPen(true);
    }
    else if ((last_y - y) > y_calibration) {
      elevationMotor->step(y_step, BACKWARD, SINGLE);
      last_y = y;
      elevation -= 1;
      adjustPen(true);
    }
    azimuth = p3x;
    elevation = p3y + 5;
    // this  "fixes" rounding errors
  }
}


void drawEllipse(int centerx, int centery, int a, int b) {
  moveTo(centerx - a, centery);
  adjustPen(true);
  
  double last_x = 0.0;
  double last_y = 0.0;
  for (double t = 0.0; t <= 1.0; t += 0.0001) {
    double x = centerx + a * cos(t * 2 * pi);
    double y = centery + b * sin(t * 2 * pi);
    if ((x - last_x) > x_calibration) {
      rotationMotor->step(x_step, FORWARD, SINGLE);
      last_x = x;
      azimuth += 1;
    }
    else if ((last_x - x) > x_calibration) {
      rotationMotor->step(x_step, BACKWARD, SINGLE);
      last_x = x;
      azimuth -= 1;
    }
    if ((y - last_y) > y_calibration) {
      elevationMotor->step(y_step, FORWARD, SINGLE);
      last_y = y;
      elevation += 1;
      adjustPen(true);
    }
    else if ((last_y - y) > y_calibration) {
      elevationMotor->step(y_step, BACKWARD, SINGLE);
      last_y = y;
      elevation -= 1;
      adjustPen(true);
    }
    azimuth = centerx - a;
    elevation = centery;
  }
}
