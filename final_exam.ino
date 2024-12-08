#include <DHT11.h>
#include <LiquidCrystal.h>

const int ENABLE_PIN = 8, RS_PIN = 7, D4_PIN = 9, D5_PIN = 10, D6_PIN = 11, D7_PIN = 12;
const int MOTOR_PIN = 3;
const int DHT11_PIN = 2;

DHT11 dht11(DHT11_PIN);
const int DHT11_ERROR_TRESHOLD = 3;
int DHT11_ERROR_COUNT = 0;

int HUMIDITY_TRESHOLD = 65;

LiquidCrystal lcd(RS_PIN, ENABLE_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);

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

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  
  lcd.createChar(0, degree_symbol);
  lcd.createChar(1, percentage_symbol);

  Serial.begin(9600);
}

void loop() {
  int temperature = 0;
  int humidity = 0;

  // Attempt to read the temperature and humidity values from the DHT11 sensor.
  int result = dht11.readTemperatureHumidity(temperature, humidity);

  // Check the results of the readings.
  // If the reading is successful, print the temperature and humidity values.
  // If there are errors, print the appropriate error messages.
  if (result == 0) {
      DHT11_ERROR_COUNT = 0;
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.print(" °C\tHumidity: ");
      Serial.print(humidity);
      Serial.println(" %");
      lcd.setCursor(0, 0);
      lcd.print("Temp.: ");
      lcd.print(temperature);
      lcd.print(" ");
      lcd.write(byte(0));
      lcd.print("C");
      lcd.setCursor(0, 1);
      lcd.print("Humidity: ");
      lcd.print(humidity);
      lcd.write(byte(1));
  } else {
      DHT11_ERROR_COUNT += 1;
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.print(" °C\tHumidity: ");
      Serial.print(humidity);
      Serial.println(" %");
      Serial.println(DHT11::getErrorString(result));
  }

  Serial.print("Humidity Treshold: ");
  Serial.println(String(HUMIDITY_TRESHOLD));
  Serial.print("Is Humidity OK: ");
  Serial.print(humidity > HUMIDITY_TRESHOLD);
  Serial.print(", Is Error OK: ");
  Serial.println(DHT11_ERROR_COUNT < DHT11_ERROR_TRESHOLD);

  if (humidity >= HUMIDITY_TRESHOLD && DHT11_ERROR_COUNT < DHT11_ERROR_TRESHOLD) {
    analogWrite(MOTOR_PIN, 255);
  } else {
    analogWrite(MOTOR_PIN, 0);
  }
}