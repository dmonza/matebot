#include <Servo.h>

// #include <AFMotor.h>
//AF_DCMotor motor(2, MOTOR12_64KHZ); // create motor #2, 64KHz pwm


Servo servoMain; // Define our Servo

void setup() {
  servoMain.attach(10);
  // put your setup code here, to run once:
  //Serial.begin(9600);
  //Serial.println("Motor test!");
  //motor.setSpeed(200); 
}

void loop() {
  servoMain.write(45);

  delay(500);
  
  servoMain.write(5);

  delay(500);
  
  /*
  // put your main code here, to run repeatedly:
  /Serial.print("tick");
  motor.run(FORWARD);
  // turn it on going forward
  delay(1000);
  Serial.print("tock");
  motor.run(BACKWARD);     // the other way  
  delay(1000);    
  Serial.print("tack");  
  motor.run(RELEASE);      // stopped  
  delay(1000);
  */
}
