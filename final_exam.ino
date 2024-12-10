/*
  The Humidity Fan

  At every loop, it use DHT11 sensor to detect temperature and humidity of the enviroment and display that on an LCD screen.
  If humidity percentage is greater or equal to 65, a DC motor starts to move the attached fan.

  This project was made with:
  - Arduino UNO
  - Power Supply Module or External Battery
  - Breadboard
  - DC Motor with Fan
  - DHT11 sensor
  - LCD Screen
  - Diode
  - NPN transistor
  - 330Ω resistence
  - 220Ω resistence
  - Potentiometer
  - Wires
  and pins were connected to used component in this way
  - Digital Pin 2: DHT11 sensor
  - Digital Pin 3: DC Motor
  - Digital Pin 7: LCD Screen RS
  - Digital Pin 8: LCD Screen Enable
  - Digital Pin 9: LCD Screen D4
  - Digital Pin 10: LCD Screen D5
  - Digital Pin 11: LCD Screen D6
  - Digital Pin 12: LCD Screen D7

  To make all components work, I used some external libraries:
  - LiquidCrystal by Arduino, Adafruit
  - DHT11 by Dhruba Saha
  downloadable directly from Arduino IDE Library Manager

  modified 10 Dec 2024
  by Davide Barbagallo
*/

#include <DHT11.h> // Include header file of DHT11 library
#include <LiquidCrystal.h> // Include header file of LiquidCrystal library

const int ENABLE_PIN = 8, RS_PIN = 7, D4_PIN = 9, D5_PIN = 10, D6_PIN = 11, D7_PIN = 12; // Declare LCD screen pins
const int MOTOR_PIN = 3; // Declare DC motor pin
const int DHT11_PIN = 2; // Declare DHT11 sensor pin

DHT11 dht11(DHT11_PIN); // Instantiate a DHT11 object which we can use to control the sensor
const int DHT11_ERROR_THRESHOLD = 3; // Declare a error threshold that we can avoid to display error of DHT11 sensor
int DHT11_ERROR_COUNT = 0; // Declare an related error count to DHT11 error threshold

int HUMIDITY_THRESHOLD = 65; // Declare an humidity threshold that we can use to define when activate our DC motor

LiquidCrystal lcd(RS_PIN, ENABLE_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN); // Instantiate a LiquidCrystal object which we can use to control the screen

// Declare a degree symbol (°) which we have to use to compatibility problem with LCD screen
byte degree_symbol[8] = {
  0b00110,
  0b01001,
  0b01001,
  0b00110,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

// Declare a percentage symbol (%) which we have to use to compatibility problem with LCD screen
byte percentage_symbol[8] = {
  0b11000,
  0b11001,
  0b00010,
  0b00100,
  0b01000,
  0b10011,
  0b00011,
  0b00000
};

// the setup function runs once when you press reset or power the board
void setup() {
  lcd.begin(16, 2); // Define width / height of LCD screen
  lcd.clear(); // Clear every screen from LCD screen
  
  lcd.createChar(0, degree_symbol); // Associate byte 0 to degree character
  lcd.createChar(1, percentage_symbol); // Associate byte 1 to percentage character

  Serial.begin(9600); // Sets the data rate for serial data transmission
}

// the loop function runs over and over again forever
void loop() {
  int temperature = 0; // Define a variable to store read temperature
  int humidity = 0; // Define a variable to store read humidity

  int result = dht11.readTemperatureHumidity(temperature, humidity); // Attempt to read the temperature and humidity values from the DHT11 sensor

  if (result == 0) { // If results == 0, then no error occurred
      DHT11_ERROR_COUNT = 0; // Reset DHT11 error count
      // Print read data on serial (useful for debugging)
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.print(" °C\tHumidity: ");
      Serial.print(humidity);
      Serial.println(" %");
      // Print read data on LCD screen
      lcd.setCursor(0, 0); // Set cursor position to first line of LCD screen
      lcd.print("Temp.: ");
      lcd.print(temperature);
      lcd.print(" ");
      lcd.write(byte(0)); // Print degree symbol of LCD screen
      lcd.print("C");
      lcd.setCursor(0, 1); // Set cursor position to second line of LCD screen
      lcd.print("Humidity: ");
      lcd.print(humidity);
      lcd.write(byte(1)); // Print percentage symbol of LCD screen
  } else { // If results != 0 then an error occurred and we cannot read new data
      DHT11_ERROR_COUNT += 1; // Increment error count
      // Print read data on serial (useful for debugging)
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.print(" °C\tHumidity: ");
      Serial.print(humidity);
      Serial.println(" %");
      Serial.println(DHT11::getErrorString(result)); // Print DHT11 error
  }

  // If read humidity is greater or equal to HUMIDITY_THRESHOLD (defined above) AND there aren't enough consecutive errors, then we can start the motor
  if (humidity >= HUMIDITY_THRESHOLD && DHT11_ERROR_COUNT < DHT11_ERROR_THRESHOLD) {
    analogWrite(MOTOR_PIN, 255); // Give a PWM pin an analogic value (255 in this case). It is not necessary to call pinMode before calling this function
  } else { // If read humidity is less then HUMIDITY_THRESHOLD (defined above) OR there was enough consecutive errors, then we can stop the motor
    analogWrite(MOTOR_PIN, 0); // Give a PWM pin an analogic value (0 in this case). It is not necessary to call pinMode before calling this function
  }
}