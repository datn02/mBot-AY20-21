
#include "MeMCore.h"

//CODE FOR MUSIC
#define NOTE_B0 31
#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_DS1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_AS1 58
#define NOTE_B1 62
#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_DS2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976
#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_C8 4186
#define NOTE_CS8 4435
#define NOTE_D8 4699
#define NOTE_DS8 4978

MeBuzzer buzzer;

// notes in the melody:
int melody[] = {
  NOTE_G4, //5  
  NOTE_G4, //5
  NOTE_A4, //6
  NOTE_G4, //5
  NOTE_C5, //1.
  NOTE_B4, //7
  0,
  NOTE_G4, //5
  NOTE_G4, //5
  NOTE_A4, //6
  NOTE_G4, //5
  NOTE_D5, //2.
  NOTE_C5, //1.
  0,
  NOTE_G4, //5
  NOTE_G4, //5
  NOTE_G5, //5.
  NOTE_E5, //3.
  NOTE_C5, //1.
  NOTE_B4, //7
  NOTE_A4, //6
  0,
  NOTE_F5, //4.
  NOTE_F5, //4.
  NOTE_E5, //3.
  NOTE_C5, //1.
  NOTE_D5, //2.
  NOTE_C5, //1.
  0,
};

int noteDurations[] = {
  8,
  8,
  4,
  4,
  4,
  4,
  4,
  8,
  8,
  4,
  4,
  4,
  4,
  4,
  8,
  8,
  4,
  4,
  4,
  4,
  2,
  8,
  8,
  8,
  4,
  4,
  4,
  2,
  4,
};

void play() {
  for (int thisNote = 0; thisNote < 29; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    buzzer.tone(melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    buzzer.noTone();
  }
}

//CODE FOR LIGHT SENSOR
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

MeUltrasonicSensor ultraSensor(PORT_1);
MeLineFollower lineFinder(PORT_4); /* Line Finder module can only be connected to PORT_3, PORT_4, PORT_5, PORT_6 of base shield. */
//int sensorState = lineFinder.readSensors();

void turn_right() {
  Left_Motor.run(-turn_speed);
  Right_Motor.run(-turn_speed);
  delay(330);
  Left_Motor.stop();
  Right_Motor.stop();
}

void turn_left() {
  Left_Motor.run(turn_speed);
  Right_Motor.run(turn_speed);
  delay(330);
  Left_Motor.stop();
  Right_Motor.stop();
}

void go_straight() {
  Left_Motor.run(-1 * left_move_speed);
  Right_Motor.run(move_speed);
}

void colourAlgorithm() {
  // Initialize serial communication using 9600 bps
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
    Serial.print("R: ");
    Serial.print(result[0]);
    Serial.print("\t");
    Serial.print("G: ");
    Serial.print(result[1]);
    Serial.print("\t");
    Serial.print("B: ");
    Serial.print(result[2]);
    Serial.print("\n");
    Serial.print("red");
    Serial.print("\n");
    colourCheck = 1;
  } else if (red > green && red > blue + 80) {
    Serial.print("R: ");
    Serial.print(result[0]);
    Serial.print("\t");
    Serial.print("G: ");
    Serial.print(result[1]);
    Serial.print("\t");
    Serial.print("B: ");
    Serial.print(result[2]);
    Serial.print("\n");
    Serial.print("yellow");
    Serial.print("\n");
    colourCheck = 2;
  } else if (red > green + 60 && red < blue) {
    Serial.print("R: ");
    Serial.print(result[0]);
    Serial.print("\t");
    Serial.print("G: ");
    Serial.print(result[1]);
    Serial.print("\t");
    Serial.print("B: ");
    Serial.print(result[2]);
    Serial.print("\n");
    Serial.print("purple");
    Serial.print("\n");
    colourCheck = 3;
  } else if (green > blue && green > red) {
    Serial.print("R: ");
    Serial.print(result[0]);
    Serial.print("\t");
    Serial.print("G: ");
    Serial.print(result[1]);
    Serial.print("\t");
    Serial.print("B: ");
    Serial.print(result[2]);
    Serial.print("\n");
    Serial.print("green");
    Serial.print("\n");
    colourCheck = 4;
  } else if (blue > green && blue > red) {
    Serial.print("R: ");
    Serial.print(result[0]);
    Serial.print("\t");
    Serial.print("G: ");
    Serial.print(result[1]);
    Serial.print("\t");
    Serial.print("B: ");
    Serial.print(result[2]);
    Serial.print("\n");
    Serial.print("blue");
    Serial.print("\n");
    colourCheck = 5;
  } else {
    Serial.print("R: ");
    Serial.print(result[0]);
    Serial.print("\t");
    Serial.print("G: ");
    Serial.print(result[1]);
    Serial.print("\t");
    Serial.print("B: ");
    Serial.print(result[2]);
    Serial.print("\n");
    Serial.print("black");
    Serial.print("\n");
    colourCheck = 6;
  }
  if (colourCheck == 6) {
    while (true) play();
  }
  Left_Motor.run(left_move_speed);
  Right_Motor.run(-1 * move_speed);
  delay(200);
  Left_Motor.stop();
  Right_Motor.stop();
  switch (colourCheck) {
    case 1: {
      turn_left();
      break;
    }
    case 2: {
      turn_left();
      turn_left();
      break;
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
 
}

void loop() {
  value_left = analogRead(IR_LEFT);
  value_right = analogRead(IR_RIGHT);

  voltage_left = value_left * (5.0f / 1023);
  voltage_right = value_right * (5.0f / 1023);
  if (lineFinder.readSensors() == S1_IN_S2_IN) {
    Left_Motor.stop();
    Right_Motor.stop();
    colourAlgorithm();
  }
  else {
    Left_Motor.run(-1 * left_move_speed);
    Right_Motor.run(move_speed);
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
        if (voltage_left - 3.20 < 0.00001) {
          break;
        }
      }
    } else if (voltage_right - 3.10 < -0.00001) {
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
        if (voltage_right - 3.10 < 0.00001) {
          break;
        }
      }
    }
  }
}
