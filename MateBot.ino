/*
Arduino Servo Test sketch
*/
#include <AFMotor.h>

AF_DCMotor motor1(1, MOTOR12_64KHZ);
AF_DCMotor motor2(2, MOTOR12_64KHZ);

#include <Servo.h>
Servo servoMain; // Define our Servo

// servo
const int servoPin = 10;
const int minangle = 5;
const int maxangle = 148;


// ultrasonic sensor
const int trigPin = 2;
const int echoPin = 13;
const int minDis = 25;
const int maxDis = 300;


// Sonido
const int soundPin = 5;

// Motors
const int motorSpeed = 200;

// Photo
const int photoValue = 200;

// state vars
long lastPlate;
int botState = 0;

int motorDir=1;

void setup() {
  // Beep PIN
  pinMode(soundPin, OUTPUT);
  
  Serial.begin(9600);
  
  photo(); // Check photo sensor
  
  // Ultrasonic sensor initialize serial communication:  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Inicialización de plato  
  movPlate();
  
  
  // turn on motors
  motor1.setSpeed(motorSpeed);
  motor2.setSpeed(motorSpeed);
  
  play();
}


void loop() {
  int d = 0;
  
  Serial.println(botState);
  // put your main code here, to run repeatedly:
  
  switch(botState){
    
    case 0: // esperando mate
        if (photo()<photoValue){
           delay(2000); // tiempo mientras acomoda el mate
           // movPlate();
           move(motorDir); // MOVE
           botState=1;
        }
        break;
        
     case 1:
        d = getDistance();
        if (d<minDis || d > maxDis)
        { 
           move(0); // STOP
           
           movPlate();
           play();
           botState=2;
        } 
        break;
        
     case 2:
       // Esperando que quite el mate del canasto
       if (photo()>photoValue){
          motorDir=motorDir*-1;
          botState=0;
        }
        break;
        
  }

  delay(50);
}

long getDistance()
{
  long duration=0;
  long avgcm=0;

  // Promerdio las X últimas lecturas
  long count = 5;
  
  for(long i=0; i<count; i++){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH); 
    delayMicroseconds(10);
    
    digitalWrite(trigPin, LOW);
  
    duration = pulseIn(echoPin, HIGH);
    avgcm +=  duration/58.2;
    delay(25);
  }

  avgcm = avgcm/count;
  
  Serial.print(avgcm);
  Serial.print("cm");
  Serial.println();
    
  return avgcm;
}

void movPlate(){
  servoMain.attach( servoPin ); // servo on digital pin
  
  if (lastPlate==minangle)
    lastPlate = maxangle;
  else
    lastPlate = minangle;

  if (lastPlate==maxangle){
    Serial.println("MIN angle");
      for(long i=lastPlate ; i>=minangle; i-=5){
        servoMain.write(i);
        delay(50);
      }
  }
  else{
    Serial.println("MAX angle");
    for(long i=lastPlate ; i<=maxangle; i+=5){
      servoMain.write(i);
      delay(50);
    }
  } 
  
  servoMain.detach();
}

void play(){
  Serial.print("Play on: ");
  Serial.println(soundPin);     // the raw analog reading
  // No usar Tone porque interfiere con pin 3 y 11
  
  analogWrite(soundPin, 10);      // Almost any value can be used except 0 and 255
  delay(400);
  analogWrite(soundPin, 50);
  delay(400);
  analogWrite(soundPin, 140);
  delay(400);
    
  analogWrite(soundPin, 0);       // 0 turns it off  
  // return;

  

  /*
  for(int i=500;i<1400;i+=100){
      tone(piezoPin, i);
      delay(200);
  }
  noTone(piezoPin);
  */
}


// Photo sensor
const int photocellPin = 5;     // the cell and 10K pulldown

int photo() {
  int photocellReading = analogRead(photocellPin);
  Serial.print("Analog reading = ");
  Serial.println(photocellReading);     // the raw analog reading

  return photocellReading;
}


void move(int dir){
  Serial.print("Move: ");
  Serial.println(dir);
  
  switch(dir){
    case 1:
      motor1.run(FORWARD);
      motor2.run(FORWARD);
      break;

    case -1:
      motor1.run(BACKWARD);
      motor2.run(BACKWARD);
      break;
            
    default:
      motor1.run(RELEASE);
      motor2.run(RELEASE);
      break;   
  }
}


