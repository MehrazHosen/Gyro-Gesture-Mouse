#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
#include "Mouse.h"

#define button 13    //left-button
#define button2 12   //right-button

boolean buttonState;
boolean buttonState2;



MPU6050 mpu;
int16_t ax, ay, az, gx, gy, gz;
int vx, vy, vz, vx_prec, vy_prec;
void setup() {

  pinMode(button, INPUT_PULLUP);
  pinMode(button2,INPUT_PULLUP);
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    while (1);
  }
}

void loop() {
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  vx = (gx + 300) / 200; // "+300" because the x axis of gyroscope give values about -350 while it's not moving. Change this value if you get something different using the TEST code, chacking if there are values far from zero.
  vy = -(gz - 100) / 250; // same here about "-100"
  vz = (gy - 200) / 100;

  Serial.print("gx = ");
  Serial.print(gx);
  Serial.print(" | gz = ");
  Serial.print(gz);

  Serial.print("        | X = ");
  Serial.print(vx);
  Serial.print(" | Y = ");
  Serial.print(vy);
  Serial.print(" | vz = ");
  Serial.print(vz);

  buttonState = digitalRead(button);
  buttonState2 = digitalRead(button2);
  
  Serial.print(" | button = ");
  Serial.print(buttonState);
  Serial.print(" | button2 = ");
  Serial.println(buttonState2);
  
  if (buttonState == 0) {
    Mouse.press();
    if ((vx > 6 || vx < -6) || (vy > 6 || vy < -6)) {
      Mouse.move(vy/1.5, -vx/1.5);
    }
  }
  else if (buttonState == 1){
    Mouse.release(MOUSE_LEFT);
    if(buttonState2 == 0){
      Mouse.press(MOUSE_RIGHT);
      if((vx > 6 || vx < 6) || (vy > 6 || vy < -6)){
        Mouse.move(0, -vx*2);
      }
    }
    else if(buttonState2 == 1){
      Mouse.release(MOUSE_RIGHT);
      if((vx > 6 || vx < 6) || (vy > 6 || vy < -6)){
        Mouse.move(vy/1.5, -vx/1.5);
      }
    }
  }
  delay(20);
}
