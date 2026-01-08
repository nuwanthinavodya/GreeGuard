// Including Libraries
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Servo.h>
#include <dht11.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// RemoteXY select connection mode and include library
#define REMOTEXY_MODE__ESP8266_HARDSERIAL_POINT


// RemoteXY connection settings
#define REMOTEXY_SERIAL Serial
#define REMOTEXY_SERIAL_SPEED 115200
#define REMOTEXY_WIFI_SSID "RemoteXY"
#define REMOTEXY_WIFI_PASSWORD "12345678"
#define REMOTEXY_SERVER_PORT 6377


#include <RemoteXY.h>

// -------------- Sensor & Module Pins --------------
#define DHT11PIN 4
#define sensorPin A0
#define red 2
#define buzzer 6
#define relayPin 12
#define motionSensor 5
#define waterLevelVCC 7
#define waterSensorPin A3
#define ldr A2

int waterLevel = 0;
int state = LOW;
int val = 0;
int soilMoisture = 0;
const int light_on  = 400;
const int light_off = 100;
bool isDay = false;

// ------------------- Components -------------------
dht11 DHT11;
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myServo;
SoftwareSerial mySerial(10, 11);
DFRobotDFPlayerMini myDFPlayer;

// -------------- Function Declarations -------------
void printingOnLCD();
void calculatingWaterLevel();
int readSensor();
void wateringPlant();
void motionDetection();
void speaker();


// RemoteXY GUI configuration
#pragma pack(push, 1)
uint8_t const PROGMEM RemoteXY_CONF_PROGMEM[] =   // 80 bytes V19
{ 255, 0, 0, 8, 0, 73, 0, 19, 0, 0, 0, 0, 145, 1, 106, 200, 1, 1, 4, 0,
  129, 22, 40, 64, 10, 64, 16, 83, 111, 105, 108, 32, 77, 111, 105, 115, 116, 117, 114, 101,
  32, 58, 0, 67, 33, 60, 40, 13, 73, 16, 1, 129, 24, 98, 57, 10, 64, 16, 87, 97,
  116, 101, 114, 32, 76, 101, 118, 101, 108, 32, 58, 0, 67, 33, 120, 40, 13, 73, 16, 1
};

// this structure defines all the variables and events of your control interface
struct {

  // output variables
  float SoilValue;
  float WaterValue;

  // other variable
  uint8_t connect_flag; 
} RemoteXY;
#pragma pack(pop)



// ---------------------- Setup ---------------------
void setup() {

  RemoteXY_Init ();  

  Serial.begin(9600);

  // LCD
  lcd.init();
  lcd.backlight();

  // Water Level Sensor + LED
  pinMode(waterLevelVCC, OUTPUT);
  digitalWrite(waterLevelVCC, LOW);
  pinMode(red, OUTPUT);


  // Buzzer
  pinMode(buzzer, OUTPUT);

  // Servo
  myServo.attach(3);

  delay(100);

  // Relay for pump
  digitalWrite(relayPin, LOW);
  pinMode(relayPin, OUTPUT);

  // Motion Sensor
  pinMode(motionSensor, INPUT);

  // MP3 player
  mySerial.begin(9600);
  if (!myDFPlayer.begin(mySerial)) {
    while (true);
  }
  myDFPlayer.volume(20);

}


// --------------------- Loop ---------------------
void loop() {

  printingOnLCD();
  calculatingWaterLevel();
  wateringPlant();
  motionDetection();
  speaker();

  RemoteXYEngine.handler ();


  digitalWrite(waterLevelVCC, HIGH);
  RemoteXYEngine.delay(10);
  waterLevel = analogRead(waterSensorPin);
  digitalWrite(waterLevelVCC, LOW);

  RemoteXY.WaterValue = waterLevel;

  int sensorValue = analogRead(sensorPin);
  int soilMoisture = map(sensorValue, 0, 1023, 100, 0);

  RemoteXY.SoilValue = soilMoisture;

//  RemoteXYEngine.delay(5000);

}


// ----------------- LCD Function -----------------
void printingOnLCD() {

  // Reading Temperature & Humidity
  DHT11.read(DHT11PIN);

  // Humidity & Temp display
  lcd.setCursor(0, 0);
  lcd.print("Hum. (%): ");
  lcd.print((float)DHT11.humidity, 2);

  lcd.setCursor(0, 1);
  lcd.print("Temp.(C):  ");
  lcd.print((float)DHT11.temperature, 2);
  delay(2000);

  lcd.clear();

  // Soil Moisture Display
  lcd.setCursor(0, 0);
  lcd.print("Moisture : ");
  lcd.print(readSensor());
  lcd.print("%");
  delay(2000);

  lcd.clear();
}


// ---------- Water Level + Servo + Buzzer -----------
void calculatingWaterLevel() {

  // Claculating water level
  digitalWrite(waterLevelVCC, HIGH);
  delay(10);
  waterLevel = analogRead(waterSensorPin);
  digitalWrite(waterLevelVCC, LOW);

  Serial.print(waterLevel);

  if (waterLevel < 200) {

    // Servo at empty level
    myServo.write(45);

    // Red LED alert
    digitalWrite(red, HIGH);

    // LCD alert
    lcd.setCursor(0, 0);
    lcd.print("Water Level is ");
    lcd.setCursor(0, 1);
    lcd.print("Low!");

    // Buzzer alert
    tone(buzzer, 1000);
    delay(2000);
    noTone(buzzer);
    //    delay(1000);

    lcd.clear();
  }
  else {
    digitalWrite(red, LOW);
    myServo.write(120);
    noTone(buzzer);
  }

  delay(500);
}

// ---------------- Relay Function ----------------
void wateringPlant() {

  soilMoisture = readSensor();

  if (soilMoisture < 30) {
    digitalWrite(relayPin, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("Watering Plant!");
  } else {
    digitalWrite(relayPin, LOW);
  }

}

// ------------ Motion Sensor Function ------------
void motionDetection() {
  val = digitalRead(motionSensor);

  if (val == HIGH) {

    if (state == LOW) {
      state = HIGH;
      lcd.setCursor(0, 0);
      lcd.print("Hello There...");
      delay(5000);

      lcd.clear();
    }
  }
  else {

    if (state == HIGH) {
      state = LOW;
    }
  }
}


// ------------ Soil Moisture Function ------------
int readSensor() {
  int sensorValue = analogRead(sensorPin);
  int soilMoisture = map(sensorValue, 0, 1023, 100, 0);
  return soilMoisture;
}

// --------------- Speaker Function ---------------

void speaker() {
  int ldrValue = analogRead(ldr);

  if (ldrValue > light_on && !isDay) {
    myDFPlayer.play(1);
    isDay = true;
  }

  if (ldrValue < light_off && isDay) {
    myDFPlayer.play(3);
    isDay = false;
  }
}
