#include <DHT.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define DHTPIN_1 2       // Digital pin connected to the first DHT sensor
#define DHTPIN_2 4       // Digital pin connected to the second DHT sensor
#define DHTTYPE DHT11    // DHT 11

#define RELAY_PIN_1 5   // Digital pin connected to the first relay
#define RELAY_PIN_2 18   // Digital pin connected to the second relay

DHT dht_1(DHTPIN_1, DHTTYPE);
DHT dht_2(DHTPIN_2, DHTTYPE);

// Set the LCD address (typically 0x27 for a 16x2 LCD)
#define LCD_ADDRESS 0x27
#define LCD_COLS 16
#define LCD_ROWS 2

// Create LCD instance
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

void setup() {
  Serial.begin(9600);
  
  // Initialize the LCD with 16 columns and 2 rows
  lcd.init();
  lcd.backlight(); // Turn on backlight
  lcd.clear(); // Clear LCD screen
  
  dht_1.begin();
  dht_2.begin();
  
  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT);
}

void loop() {
  delay(2000);  // Wait for 2 seconds between measurements
  
  // Read temperature and humidity for the first DHT sensor
  float temperature_1 = dht_1.readTemperature();

  // Read temperature and humidity for the second DHT sensor
  float temperature_2 = dht_2.readTemperature();

  if (isnan(temperature_1) || isnan(temperature_2)) {
    Serial.println("Failed to read from DHT sensor!");
    lcd.clear(); // Clear the LCD if reading fails
    lcd.setCursor(0, 0);
    lcd.print("Failed to read DHT");
    return;
  }

  // Print temperature values for both sensors on the LCD
  lcd.clear(); // Clear the LCD before printing new values
  lcd.setCursor(0, 0);
  lcd.print("T1: ");
  lcd.print(temperature_1);
  lcd.print("C");
  
  lcd.setCursor(0, 1);
  lcd.print("T2: ");
  lcd.print(temperature_2);
  lcd.print("C");

  // Print temperature values for both sensors on Serial Monitor
  Serial.print("Battery Temperature: ");
  Serial.print(temperature_1);
  Serial.println(" °C");

  Serial.print("Powertrain Temperature: ");
  Serial.print(temperature_2);
  Serial.println(" °C");

  // Check if temperature for Battery exceeds 37°C, turn on relay 1
  if (temperature_1 > 37) {
    digitalWrite(RELAY_PIN_1, HIGH);  // Turn on relay 1
    Serial.println("Temperature for Battery exceeded 37°C. Turning on relay 1.");
  } else {
    digitalWrite(RELAY_PIN_1, LOW);   // Turn off relay 1
  }

  // Check if temperature for powertrain exceeds 38°C, turn on relay 2
  if (temperature_2 > 38) {
    digitalWrite(RELAY_PIN_2, HIGH);  // Turn on relay 2
    Serial.println("Temperature for powertrain exceeded 38°C. Turning on relay 2.");
  } else {
    digitalWrite(RELAY_PIN_2, LOW);   // Turn off relay 2
  }
}
