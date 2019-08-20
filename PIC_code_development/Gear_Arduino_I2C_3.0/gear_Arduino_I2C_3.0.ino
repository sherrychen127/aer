/* Arduino_I2C_PIC
 *  
 * Author: Michael Ding
 * Date: Summer 2016
 * Edited by Tyler Gamvrelis, summer 2017
 * Description: Demonstration of Arduino-PIC interfacing via I2C. In this program, the PIC outputs
 *              keypad data to the Arduino, which forwards it to a PC. If a triple-A sequence is
 *              given on the keypad, then the PIC will display data entered into the serial monitor
 *              on the character LCD. To open the serial monitor, go to Tools -> Serial Monitor.
 * Preconditions:
 *   1. PIC-Arduino link switches are enabled (ON) for A4 and A5 of the Arduino
 *      Nano (RC4 and RC3 for the primary PIC, respectively)
 *   2. PIC and Arduino Nano agree on the Arduino Nano's slave address (i.e. the
 *      same slave address is used in software)
 *   3. A PC is connected if the Arduino Nano serial monitor is to be used
 */

#include <Wire.h>
#include <Servo.h> 

Servo myservo_B;
Servo myservo_N;
Servo myservo_S;
Servo myservo_W;
Servo con_servo;

int DC_motor0 = 2;    //PWM pins   
int DC_motor1 = 5;
int DC_motor2 = 6;
int DC_motor3 = 3; //9

int IR_sensor = 4;  //pin
int IR_sensor_val = 0;

int con_servo_pin = 9;

int servo_motor0 = 7;
int servo_motor1 = 11;
int servo_motor2 = 12;
int servo_motor3 = 8;

int servo_BR = A1;

int motor_speed = 170;

int prev = 0;   //prev sensing value, 1 means fasteners pass through
int data_send = 0;

int gear_counter = 0;
int num = 0;
int analog_result; //gear_BR counter
int gear_rotate = 0;
int val;



void receiveEvent(void);

void requestEvent(void);

void setup(){
  myservo_B.attach(servo_motor0);
  myservo_N.attach(servo_motor1);
  myservo_S.attach(servo_motor2);
  myservo_W.attach(servo_motor3);
  
  con_servo.attach(con_servo_pin);
  

  // bolts natrual 90 ->0
//washer natrual   150 -> 70
//nuts natrual    60 -> 0
//spacer       80-> 0
  

  myservo_B.write(90); 
  myservo_N.write(60); 
  myservo_S.write(80); 
  myservo_W.write(150); 

  pinMode(DC_motor0,OUTPUT);
  pinMode(DC_motor1,OUTPUT);
  pinMode(DC_motor2,OUTPUT);
  pinMode(DC_motor3,OUTPUT);

  pinMode(IR_sensor,INPUT);


  pinMode(A0,INPUT);
  pinMode(servo_BR,INPUT);  //keep track of gear teeth
  
  
  delay(1000);    
  Wire.begin(8); // Join I2C bus with address 8
  Wire.onReceive(receiveEvent); // Register a function to be called when this slave device receives a transmission from a master
  Wire.onRequest(requestEvent); // Register a function to be called when a master requests data from this slave device

  Serial.begin(9600); // Open serial port to PC (hardware UART)    
  
}



uint8_t state = 0;
uint8_t incomingByte;


void loop(){
  
  
  //while(1){
    //con_servo.write(30);
  //}
  
  //Serial.println(data_send);
  
  
  if (state && Serial.available() > 0 && !incomingByte) {
    incomingByte = Serial.read();
  }
 Serial.println("gear_rotate");
 Serial.print(gear_rotate);
  if (gear_rotate == 1){
    con_servo.write(83);
    while(1){
      
      val = analogRead(A0);
      //Serial.println(val);
      if (num >= 50){
        break;
      }
      if (val<=260)
      {
        if (prev == 0){
          
          num++;
          Serial.println(num);
        }

        prev = 1;
        continue;
    
      }
      else if (val> 260){
        prev = 0;
      }
    }
    con_servo.write(90);
    


    
    gear_rotate = 0;

    
  }
  num = 0;
  con_servo.write(90);
  delay(800);

  con_servo.write(83);
  /*
  while(1){
    IR_sensor_val = digitalRead(IR_sensor);
    printf(IR_sensor_val);
  }
  */
  
  
}


uint8_t buf[3];
uint8_t counter = 0;  



void receiveEvent(void){
  /* This function is called whenver a transmission is received from a master.
   * 
   * Arguments: none
   * 
   * Returns: none
   */
   
  uint8_t x = Wire.read(); // Receive byte
  //Serial.println((char)x);
  gear_counter = 0;
  //printf("start rotating");
  if (x == '1'){
    //Serial.println("start rotating");
    
     gear_rotate = 1;
      return;
    }
  else{
    gear_rotate = 0;
  }
   
      
  

  
  
  if (x == 'a'){     //change to '1'
    myservo_B.write(90); 
    myservo_N.write(50); 
    myservo_S.write(80); 
    myservo_W.write(150); 
  }
  if (x == '2'){ 
    Serial.println("Servo 2");
    myservo_B.write(0); 
    myservo_N.write(0); 
    myservo_S.write(0); 
    myservo_W.write(90); 
  }

    //rotate DC_motor0
  if (x == '3'){
      analogWrite(DC_motor0,motor_speed);
      
  }
  if (x == '4'){       //stop DC_motor0
      analogWrite(DC_motor0,0);
  }
/**********************************/
  if (x == '5'){       //start DC_motor1
      analogWrite(DC_motor1,motor_speed);
  }

  if (x == '6'){       //stop DC_motor1
      analogWrite(DC_motor1,0);
  }

  /**********************************/
  if (x == '7'){       //start DC_motor2
      analogWrite(DC_motor2,motor_speed);
  }

  if (x == '8'){       //stop DC_motor2
      analogWrite(DC_motor2,0);
  }

  /**********************************/
  if (x == '9'){       //start DC_motor3
      analogWrite(DC_motor3,motor_speed );
  }

  if (x == '0'){       //stop DC_motor3
      analogWrite(DC_motor3,0);
  }




  
  
  //Serial.println((char)x); // Print to serial output as char (ASCII representation)

  //buf[counter++] = x;
  //counter = (counter == 3) ? 0 : counter;

  //if(buf[0]=='A' && buf[1]=='A' && buf[2]=='A'){
    //state = 1;
  //}
}

void requestEvent(void){
   /* This function is called whenver a master requests data.
   * 
   * Arguments: none
   * 
   * Returns: none
   */
   if (data_send){
      Wire.write('a'); // Respond with message of 1 byte
      
   }
   else{
    Wire.write('b');
   }
  
}
