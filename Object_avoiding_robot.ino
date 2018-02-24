/*   Arduino Object Avoiding Robot with Ultrasonic Sensor 
 *   Dev: Michalis Vasilakis // Date: 15/7/2017 // www.ardumotive.com 
 */

//Libraries
#include <Servo.h> 
#include "Ultrasonic.h"

//Constants - Connections
const int buzzer = 8;     
const int motorA1= 2;     
const int motorA2= 6;  
const int enableA = 3; 
const int motorB1= 10;     
const int motorB2= 9; 
const int enableB = 5;    

Ultrasonic ultrasonic(A2 ,A3); //Create Ultrsonic object ultrasonic(trig pin,echo pin)
Servo myservo;          //Create Servo object to control a servo

//Variables
int distance;         //Variable to store distance from an object
int checkRight;
int checkLeft;
int pos=100;          //Variable to store the servo position. By default 90 degrees - sensor will 'look' forward
int speedPWM = 200;   //Change speed (PWM max 255)

void setup()
{
  myservo.attach(7);      //Servo pin connected to pin 7
  myservo.write(pos);     // tell servo to go to position in variable 'pos' 
  pinMode(buzzer, OUTPUT);
  pinMode(motorA1,OUTPUT);
  pinMode(motorA2,OUTPUT);
  pinMode(motorB1,OUTPUT);
  pinMode(motorB2,OUTPUT); 
  pinMode(enableA, OUTPUT);
  pinMode(enableB, OUTPUT);
  delay(5000); //Wait 5 seconds...
}

void loop(){
    analogWrite(enableA, speedPWM);
    analogWrite(enableB, speedPWM);
    //Read distance...
    distance = ultrasonic.Ranging(CM); //Tip: Use 'CM' for centimeters or 'INC' for inches
    delay(20);
    //Check for objects...
    if (distance > 15){
      forward(); //All clear, move forward!
      noTone(buzzer);
    }
    else if (distance <=15){
      stop(); //Object detected! Stop the robot and check left and right for the better way out!
      tone(buzzer,500); // play a tone
      //Start scanning... 
      for(pos = 30; pos < 170; pos += 1){  //goes from 0 degrees to 180 degrees 
                myservo.write(pos);             //tell servo to go to position in variable 'pos' 
                delay(10);                      //waits 10ms for the servo to reach the position 
            } 
            
            checkLeft = ultrasonic.Ranging(CM);   //Take distance from the left side
            
            for(pos = 170; pos>=30; pos-=1){     //goes from 180 degrees to 0 degrees                           
                myservo.write(pos);             //tell servo to go to position in variable 'pos' 
                delay(10);                      //waits 10ms for the servo to reach the position   
            }
            
            checkRight= ultrasonic.Ranging(CM);
            
            myservo.write(100);                   // Sensor "look" forward again
            
            //Finally, take the right decision, turn left or right?
            if (checkLeft < checkRight){
              left();
              delay(500); // delay, change value if necessary to make robot turn.
              stop();
                        
              }
            else if (checkLeft > checkRight){
              right();              
              delay(500); // delay, change value if necessary to make robot turn.
              stop();
            }
            else if (checkLeft <=10 && checkRight <=10){
              backward(); //The road is closed... go back and then left ;)
              stop(); // Stop going backwards
              delay(500);
              left();
            }
    }
    delay(150);
}


void forward(){
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW); 
}

void backward(){
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, HIGH);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, HIGH);
}

void left(){
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, HIGH);
}

void right(){
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, HIGH);
  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW); 
}

void stop(){
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, LOW);
}



