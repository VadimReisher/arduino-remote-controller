#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>

// Define the digital and analog inputs
#define RightButtonPin 5      // Right Button Pin
#define LeftButtonPin 4       // Left Button Pin
#define SwRightPosPin  6      // Right togle up position Pin
#define SwLeftPosPin 7        // Right togle down position Pin
#define LeftStickX A1         // Left joystick X-axis
#define LeftStickY A2         // Left joystick Y-axis
#define RightStickX A3        // Right joystick X-axis
#define RightStickY A4        // Right joystick Y-axis

// Define repeating numbers
#define StickMiddlePoint 127


RF24 radio(10, 9);                // nRF24L01 (CE, CSN)
const byte address[6] = "00001";  // Address

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  byte rightStickX = StickMiddlePoint;    // Right joystick X axis potentiometer
  byte rightStickY = StickMiddlePoint;    // Right joystick Y axis potentiometer
  byte leftStickX = StickMiddlePoint;     // Left joystick X axis potentiometer
  byte leftStickY = StickMiddlePoint;     // Left joystick Y axis potentiometer
  byte rightButton = HIGH;                // Right joystick button
  byte leftButton = HIGH;                 // Left joystick button
  byte swLeftPos = HIGH;                  // Right side toggle upper position
  byte swRightPos = HIGH;                 // Right side toggle lower position
}data;      //Create an instance of the above struct


void setup() {
//  Serial.begin(9600);

  // Define the radio communication
  radio.begin();
  radio.openWritingPipe(address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);

  // Activate the Arduino internal pull-up resistors
  pinMode(RightButtonPin, INPUT_PULLUP);
  pinMode(LeftButtonPin, INPUT_PULLUP);
  pinMode(SwRightPosPin, INPUT_PULLUP);
  pinMode(SwLeftPosPin, INPUT_PULLUP);

}

void loop() {
  // Read all analog inputs and map them to one Byte value
  data.rightStickX = map(analogRead(RightStickX), 150, 900, 0, 255); // Convert the analog read value from 0 to 1023 into a BYTE value from 0 to 255
  data.rightStickY = map(analogRead(RightStickY), 200, 920, 0, 255);
  data.leftStickX = map(analogRead(LeftStickX), 140, 900, 0, 255);
  data.leftStickY = map(analogRead(LeftStickY), 190, 910, 0, 255);

  // Read all digital inputs
  data.rightButton = digitalRead(RightButtonPin);
  data.leftButton = digitalRead(LeftButtonPin);
  data.swLeftPos = digitalRead(SwLeftPosPin);
  data.swRightPos = digitalRead(SwRightPosPin);

  // Print all data to check values
//  Serial.print("  RJ X:");
//    Serial.print(data.rightStickX);
//    Serial.print(" RJ Y: ");
//    Serial.print(data.rightStickY);
//    Serial.print("; RB: ");
//    Serial.print(data.rightButton);
//    Serial.print(" LJ X: ");
//    Serial.print(data.leftStickX);
//    Serial.print(" LJ Y: ");
//    Serial.print(data.leftStickY);
//    Serial.print("; LB: ");
//    Serial.print(data.leftButton);
//    Serial.print("; RU Tog: ");
//    Serial.print(data.swLeftPos);
//    Serial.print("; RLo Tog: ");
//    Serial.println(data.swRightPos);


  // Send the whole data from the structure to the receiver
  radio.write(&data, sizeof(Data_Package));
}
