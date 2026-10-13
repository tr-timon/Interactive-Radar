#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Servo.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);
DHT dht(2, DHT11);
Servo myServo;

const int BTN_PREV = A0;
const int BTN_NEXT = A1;
const int BTN_ACT  = A2;

int menuPage = 0;
int servoAngle = 90;

void setup() {
  pinMode(BTN_PREV, INPUT_PULLUP);
  pinMode(BTN_NEXT, INPUT_PULLUP);
  pinMode(BTN_ACT,  INPUT_PULLUP);

  dht.begin();
  myServo.attach(9);
  myServo.write(servoAngle);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
}

void loop() {
  // Navigation controls
  if (digitalRead(BTN_NEXT) == LOW) {
    menuPage = (menuPage + 1) % 2;
    delay(200); // Simple debounce
  }
  if (digitalRead(BTN_PREV) == LOW) {
    menuPage = (menuPage - 1 + 2) % 2;
    delay(200);
  }

  display.clearDisplay();
  display.setTextColor(WHITE);

  if (menuPage == 0) {
    // Mode 0: Weather Display
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("< MODE 1: CLIMATE >");
    display.setTextSize(2);
    display.setCursor(0, 20);
    display.print("T: "); display.print(t, 1); display.println("C");
    display.setCursor(0, 42);
    display.print("H: "); display.print(h, 0); display.println("%");
  } 
  else if (menuPage == 1) {
    // Mode 1: Manual Servo Positioner
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("< MODE 2: SERVO >");
    display.setTextSize(2);
    display.setCursor(0, 25);
    display.print("Angle: "); display.print(servoAngle);

    // Action button rotates servo in increments
    if (digitalRead(BTN_ACT) == LOW) {
      servoAngle += 45;
      if (servoAngle > 180) servoAngle = 0;
      myServo.write(servoAngle);
      delay(200);
    }
  }

  display.display();
}