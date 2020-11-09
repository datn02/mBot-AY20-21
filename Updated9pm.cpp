
#include "MeMCore.h"

//CODE FOR MUSIC
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319

//Initializing the mBot's buzzer
MeBuzzer buzzer;

//Function that is called and repeated when black colour is detected
void victoryTune(){

 int melody[] = {
    NOTE_C6, 
    NOTE_C6, 
    NOTE_C6,
    NOTE_C6,
    NOTE_GS5, 
    NOTE_AS5,
    NOTE_C6,   
    0,   
    NOTE_AS5,
    NOTE_C6,
    0,
 };

 int durations[] = {
    12, 
    12, 
    12,
    4,
    4, 
    4,
    12, 
    12, 
    12,
    1,
    2,
 };

 //Each note is played sequentially
 for (int index = 0; index < 11; index++) {
    
    //Play the note with a duration pre-fixed in the durations[] array
    int duration = 1000/durations[index];
    buzzer.tone(8, melody[index], duration);
    
    //Very brief pause for the note to be played
    int pause = duration * 1.30;
    delay(pause);
    
    //Switch off the current tone
    buzzer.noTone(8);
 }

}

//CODE FOR LIGHT SENSOR

//Initializing the light sensor and LED port.
MeLightSensor lightSensor(PORT_6);
MeRGBLed led(PORT_7);

#define IR_LEFT A3
#define IR_RIGHT A2

float gain_left = 270.5f;
float gain_right = 310.5f;

int value_left, value_right;
float voltage_left, voltage_right;

MeDCMotor Left_Motor(M1);
MeDCMotor Right_Motor(M2);

float left_setpoint = 3.20f;
float right_setpoint = 3.10f;

uint8_t turn_speed = 200;
uint8_t move_speed = 210;
uint8_t left_move_speed = move_speed + 3;

double distance = 5.0f;
double stop_value = 0.01;


//Initializing the ultrasonic sensor
MeUltrasonicSensor ultraSensor(PORT_1);
MeLineFollower lineFinder(PORT_4);

//Function to turn right
void turn_right() {
  Left_Motor.run(-turn_speed);
  Right_Motor.run(-turn_speed);
  delay(330);
  Left_Motor.stop();
  Right_Motor.stop();
}

//Function to turn left
void turn_left() {
  Left_Motor.run(turn_speed);
  Right_Motor.run(turn_speed);
  delay(330);
  Left_Motor.stop();
  Right_Motor.stop();
}

//Function to go straight
void go_straight() {
  Left_Motor.run(-1 * left_move_speed);
  Right_Motor.run(move_speed);
}

//Function for the colour challenge
void colourAlgorithm() {
  Serial.begin(9600);
  lightSensor.reset(PORT_8);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  int currReading[] = {
    0,
    0,
    0
  };
  int result[] = {
    0,
    0,
    0
  };
  int reading;
  int totalRed = 0;
  int totalBlue = 0;
  int totalGreen = 0;

  //Get reading for Red value
  currReading[0] = 255;
  led.setColor(currReading[0], currReading[1], currReading[2]);
  led.show();
  //Repeat reading for 5 times, taking the average for accuracy
  for (int colour = 0; colour < 5; colour++) {
    delay(30);
    reading = lightSensor.read();
    totalRed += reading;
  }
  result[0] = totalRed / 5;
  currReading[0] = 0;
  led.setColor(0, 0, 0);
  led.show();
  //Get reading for Green value
  currReading[1] = 255;
  led.setColor(currReading[0], currReading[1], currReading[2]);
  led.show();
  //Repeat reading for 5 times, taking the average for accuracy
  for (int colour = 0; colour < 5; colour++) {
    delay(30);
    reading = lightSensor.read();
    totalGreen += reading;
  }
  result[1] = totalGreen / 5;
  currReading[1] = 0;
  led.setColor(0, 0, 0);
  led.show();

  //Get reading for Blue value
  currReading[2] = 255;
  led.setColor(currReading[0], currReading[1], currReading[2]);
  led.show();
  //Repeat reading for 5 times, taking the average for accuracy
  for (int colour = 0; colour < 5; colour++) {
    delay(30);
    reading = lightSensor.read();
    totalBlue += reading;
  }
  result[2] = totalBlue / 5;
  currReading[2] = 0;
  led.setColor(0, 0, 0);
  led.show();

  int red = result[0];
  int green = result[1];
  int blue = result[2];

  int colourCheck;

  if (red > green + 280 && red > blue + 280) {
  	//Red is detected
    colourCheck = 1;
  } else if (red > green && red > blue + 80) {
  	//Yellow is detected
    colourCheck = 2;
  } else if (red > green + 60 && red < blue) {
  	//Purple is detected
    colourCheck = 3;
  } else if (green > blue && green > red) {
  	//Green is detected
    colourCheck = 4;
  } else if (blue > green && blue > red) {
  	//Blue is detected
    colourCheck = 5;
  } else {
  	//Black is detected
    colourCheck = 6;
  }

  //If black is detected, the maze is completed and victory music plays
  if (colourCheck == 6) {
    while (true) victory_tune();
  }

  //Robot moves back slightly to prevent hitting the wall
  Left_Motor.run(left_move_speed);
  Right_Motor.run(-1 * move_speed);
  delay(200);
  Left_Motor.stop();
  Right_Motor.stop();

  //Follows the colour instructions
  switch (colourCheck) {
    case 1: {
      turn_left();
      break;
    }
    case 2: {
      value_left = analogRead(IR_LEFT);
      value_right = analogRead(IR_RIGHT);
      voltage_left = value_left * (5.0f / 1023);
      voltage_right = value_right * (5.0f / 1023);
      if (voltage_left < 3.20) {
        turn_right();
        turn_right();
        break;
      } else {
        turn_left();
        turn_left();
        break;
      }
    }
    case 3: {
      turn_left();
      //go straight until ultrasonic sensor reads a distance from the wall
      Left_Motor.run(-1 * left_move_speed);
      Right_Motor.run(move_speed);
      while (ultraSensor.distanceCm() - 10.0 > stop_value) {
        continue;
      }
      Left_Motor.stop();
      Right_Motor.stop();
      turn_left();
      break;
    }
    case 4: {
      turn_right();
      break;
    }
    case 5: {
      turn_right();
      //go straight until ultrasonic sensor reads a distance from the wall
      Left_Motor.run(-1 * left_move_speed);
      Right_Motor.run(move_speed);
      while (ultraSensor.distanceCm() - 10.0 > stop_value) {
        continue;
      }
      Left_Motor.stop();
      Right_Motor.stop();
      turn_right();
      break;
    }
  }
}

void setup() {
 //No setup code is required
}

void loop() {

  //Read the sensor value from both side of the mBot
  value_left = analogRead(IR_LEFT);
  value_right = analogRead(IR_RIGHT);

  //Calculate the voltage reading
  voltage_left = value_left * (5.0f / 1023);
  voltage_right = value_right * (5.0f / 1023);

  //If a black strip is detected, stop the mBot and do the colour challenge
  if (lineFinder.readSensors() == S1_IN_S2_IN) {
    Left_Motor.stop();
    Right_Motor.stop();
    colourAlgorithm();
  }
  //If a black strip is not detected, move the mBot forward
  //The mBot constantly checks if it is too near to the wall
  else {
    Left_Motor.run(-1 * left_move_speed);
    Right_Motor.run(move_speed);

    //If the mBot is too close to the left, adjust rightwards
    if (voltage_left - 3.20 < -0.00001) {
      Left_Motor.run(-1 * (left_move_speed + gain_left * (3.20 - voltage_left)));
      Right_Motor.run(move_speed - gain_left * (3.20 - voltage_left));
      while (true) {
        if (lineFinder.readSensors() == S1_IN_S2_IN) {
          Left_Motor.stop();
          Right_Motor.stop();
          break;
        }
        value_left = analogRead(IR_LEFT);
        voltage_left = value_left * (5.0f / 1023);
        //Once it is no longer near the left wall, stop adjusting
        if (voltage_left - 3.20 < 0.00001) {
          break;
        }
      }
    } 
    //Else, if the mBot is too close to the left, adjust rightwards
    else if (voltage_right - 3.10 < -0.00001) {
      Left_Motor.run(-1 * (left_move_speed - gain_right * (3.10 - voltage_right)));
      Right_Motor.run(move_speed + gain_right * (3.10 - voltage_right));
      while (true) {
        if (lineFinder.readSensors() == S1_IN_S2_IN) {
          Left_Motor.stop();
          Right_Motor.stop();
          break;
        }
        value_right = analogRead(IR_RIGHT);
        voltage_right = value_right * (5.0f / 1023);
        //Once it is no longer near the right wall, stop adjusting
        if (voltage_right - 3.10 < 0.00001) {
          break;
        }
      }
    }
  }
}